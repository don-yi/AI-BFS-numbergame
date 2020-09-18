#include "numbergame.h"
#include <iostream>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <tuple>
#include <fstream>

// you may reuse this code - copy-paste
class GameField {
    public:
        GameField( std::string const&  filename );
        friend std::ostream& operator<<( std::ostream& os, GameField const& pl );

        ~GameField()                                = default;
        GameField( GameField const& )               = delete; 
        GameField( GameField && )                   = delete; 
        GameField & operator=( GameField const& )   = delete; 
        GameField & operator=( GameField && )       = delete; 

        std::vector< std::tuple<unsigned, unsigned> > SolveBFS() 
        { 
            return SolveNumbergameBFS( filename ); // global function prototyped in numbergame.h
        }
        
        bool IsSolved() const;
        int Check( std::vector< std::tuple<unsigned, unsigned> > const& sol );
        int CheckBrief( std::vector< std::tuple<unsigned, unsigned> > const& sol );
        void Print( std::string const& filename_out );
        void makeMove( std::tuple< unsigned, unsigned > move, bool brief );

    private:
        unsigned            width    = 0;
        std::vector< int >  field    = std::vector< int >();    // flat array
        std::string         filename = std::string();   
};

void run_test( char const * filename )
{
    try {
        GameField pl( filename );
        std::vector< std::tuple<unsigned, unsigned> > sol; // positions in the vector
        std::cout << pl;

            sol = pl.SolveBFS( );
        //pl.Check( sol );        // for debugging ?
        pl.CheckBrief( sol );   // for grading
    } catch ( char const * msg ) {
        std::cerr << " in driver " << msg << std::endl;
        throw;
    }
}

// BFS
void test0() { run_test( "in0" ); }
void test1() { 
//    NG g( "in1" );
//    std::cout << g << std::endl;
//    g.makeMove( std::make_tuple( 1,1 ) );
//    std::cout << g << std::endl;
    run_test( "in1" ); 
    }
void test2() { run_test( "in2" ); }
void test3() { run_test( "in3" ); }
void test4() { run_test( "in4" ); }
void test5() { run_test( "in5" ); }

#include <cstdio> /* sscanf */
int main( int argc, char ** argv ) 
{
    //////////////////////////////////////////////////////////////////////
    // single command argument - run predefined test                    //
    //////////////////////////////////////////////////////////////////////
    if ( argc == 2 ) {                                                  //
        void (*pTests[])() = { test0,test1,test2,test3,test4,test5,     //
        };                                                              //
        pTests[atoi( argv[1] )]();                                      //
        return 0;                                                       //
    }                                                                   //
    //////////////////////////////////////////////////////////////////////
}



// GameField implementation
GameField::GameField( std::string const&  filename ) : filename( filename ) 
{
    std::ifstream infile ( filename );
    if ( not infile.good() ) {
        throw "Cannot open input file";
    }

    // read width
    infile >> width;

    int input;
    infile >> input;
    while ( infile.good() and not infile.eof() ) {
        field.push_back( input );
        infile >> input;
    }
}

// std::tuple< unsigned, unsigned >
// is a pair i,j of indices into the field array:
// if i == j then the move is a "duplicate" move - see rules
// if i != j then the move is a "delete pair" of neighbors - see rules
void GameField::makeMove( std::tuple< unsigned, unsigned > move, bool brief = true )
{
    unsigned pos1 = std::get<0>( move );
    unsigned pos2 = std::get<1>( move );
    if ( pos1 > pos2 ) {
        std::swap( pos1, pos2 );
    }
 
    // duplicate values
    if ( pos1 == pos2 ) {
        if ( not brief ) { std::cout << "Duplicate values\n"; }
        std::vector<int> new_values;
        for ( int v : field ) {
            if ( v != 0 ) {
                new_values.push_back( v );
            }
        }
        field.insert( field.end(), new_values.begin(), new_values.end() );
        return;
    }

    if ( field[pos1] == 0 or field[pos2] == 0 ) {
        throw "One of the positions is already deleted";
    }
    if ( not ( pos1 < field.size() and pos2 < field.size() ) ) {
        throw "position is outside of field";
    }
    if ( not ( field[pos1] == field[pos2] or field[pos1] + field[pos2] == 10 ) ) {
        throw "positions are not equal nor add to 10";
    }

    // are they horizontal neighbors?
    // start at pos1 (smaller) and go right
    bool hor_neigbors = true;
    for ( unsigned i = pos1 +1; i < pos2 and hor_neigbors; ++i ) {
        if ( field[i] != 0 ) {
            hor_neigbors = false;
        }
    }
    if ( hor_neigbors ) {
        if ( not brief ) { std::cout << "horizontal neighbors\n"; }
        field[pos1] = field[pos2] = 0; // delete
        return;
    }

    // are they vertical neighbors? 
    // slightly more complicated logic than horisontal - see extra check before for-loop
    // start at pos1 (smaller) and go down, check inside array
    bool vert_neigbors = true;

    // simple check first
    if ( (pos2-pos1) % width != 0 ) {
        throw "positions are not vertical neighbors";
    }

    for ( unsigned i = pos1 + width; i < pos2 and vert_neigbors; i+=width ) {
        if ( field[i] != 0 ) {
            vert_neigbors = false;
        }
    }
    if ( vert_neigbors ) {
        if ( not brief ) { std::cout << "vertical neighbors\n"; }
        field[pos1] = field[pos2] = 0; // delete
        return;
    }

    // if here - not neighbors
    throw "positions are not neighbors";
}

bool GameField::IsSolved() const 
{
    // return true if cannot find non-zero element in the field
    return field.end() == std::find_if( field.begin(), field.end(), []( unsigned el) { return (el != 0); } );
}

int GameField::Check( std::vector< std::tuple<unsigned, unsigned> > const& sol ) 
{
    try {
        for ( std::tuple<unsigned, unsigned> const& m : sol ) {
            makeMove( m, false );
            std::cout << *this;
        }
        bool final_pos = IsSolved( );
        std::cout << "Number of steps = " << sol.size() << std::endl;
        std::cout << "Solved = " << final_pos << std::endl;
        return final_pos;
    } catch ( char const * msg ) {
        std::cout << "ERROR - " << msg << std::endl;
        return 1;
    }
    return 0; // OK
}

int GameField::CheckBrief( std::vector< std::tuple<unsigned, unsigned> > const& sol )
{
    try {
        for ( std::tuple<unsigned, unsigned> const& m : sol ) {
            makeMove( m );
        }
        bool final_pos = IsSolved( );
        std::cout << "Number of steps = " << sol.size() << std::endl;
        std::cout << "Solved = " << final_pos << std::endl;
        return final_pos;
    } catch ( char const * msg ) {
        std::cout << "ERROR - " << msg << std::endl;
        return 1;
    }
    return 0; // OK
}

std::ostream& operator<<( std::ostream& os, GameField const& pl ) {
    for ( unsigned i=0; i<pl.field.size(); ++i ) {
        if ( i>0 and i%pl.width == 0 ) { os << std::endl; }
        if ( pl.field[i] == 0 ) {
            os << "-"         << " ";
        } else {
            os << pl.field[i] << " ";
        }
    }
    os << std::endl << std::setfill( '-' ) << std::setw( 2*pl.width -1 ) << "-" << std::endl;
    return os;
}

void GameField::Print( std::string const& filename_out )
{
    std::ofstream os;
    os.open( filename_out, std::ofstream::out | std::ofstream::app );
    os << *this;
}

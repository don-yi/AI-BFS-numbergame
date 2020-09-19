#ifndef NUMBERGAME_H
#define NUMBERGAME_H

#include <fstream>
#include <vector>
#include <queue>

// Keep this
////////////////////////////////////////////////////////////////////////////////
// global functions
std::vector< std::tuple<unsigned, unsigned> > 
SolveNumbergameBFS( std::string const& filename );

////////////////////////////////////////////////////////////////////////////////
// your stuff
////////////////////////////////////////////////////////////////////////////////
std::tuple<unsigned, unsigned>
RmPair(std::vector< int >&  field, unsigned ind, unsigned width);

bool IsChecking(std::vector< int >& field);

#endif

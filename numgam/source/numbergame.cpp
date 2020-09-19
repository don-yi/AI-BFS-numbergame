#include <tuple>
#include "numbergame.h"

std::vector<std::tuple<unsigned, unsigned>>
SolveNumbergameBFS(std::string const& filename)
{
  // GameField implementation
  unsigned            w= 0;
  std::vector< int >  field = std::vector< int >();    // flat array

  std::ifstream infile(filename);
  if (not infile.good()) {
    throw "Cannot open input file";
  }

  // read width
  infile >> w;

  int input;
  infile >> input;
  while (infile.good() and not infile.eof()) {
    field.push_back(input);
    infile >> input;
  }

  int const originalSize = field.size();


  // main bfs impl
  std::vector<std::tuple<unsigned, unsigned>> sol;

  do
  {
    // q init
    std::queue<int> q;
    q.push(0);
    while (not q.empty())
    {
      // get num ind from q
      unsigned fr = q.front();
      unsigned num = field[fr];

      // handle q
      q.pop();
      if ((fr + 1) % w and (fr + 1) < field.size() and field[fr + 1])
      {
        q.push(fr + 1);
      }
      if ((fr + w) < field.size() and field[fr + w])
      {
        q.push(fr + w);
      }

      // handle game logic
      if (not num)
      {
        continue;
      }

      std::tuple<unsigned, unsigned> pair = RmPair(field, fr, w);
      // if no pair
      if (not std::get<1>(pair))
      {
        continue;
      }
      // if paired after check
      else if (std::get<1>(pair) >= originalSize)
      {
        std::get<1>(pair) = std::get<0>(pair);
      }
      // pair found, push
      sol.push_back(pair);
    }
  } while (IsChecking(field)); // til solved (no checks)

  return sol;
}

std::tuple<unsigned, unsigned>
RmPair(std::vector<int>& field, unsigned ind, unsigned width)
{
  // search towards right
  for (size_t i = ind + 1; i < field.size(); i++)
  {
    if (field[i] == 0)
    {
      continue;
    }

    if (field[i] == field[ind] or field[i] + field[ind] == 10)
    {
      field[i] = 0;
      field[ind] = 0;
      return std::make_tuple(ind, i);
    }
    else
    {
      break;
    }
  }

    // search towards left
  // zero sub check
  if ((ind - 1) >= 0)
  {
    for (int i = ind - 1; i >= 0; --i)
    {
      if (field[i] == 0)
      {
        continue;
      }

      if (field[i] == field[ind] or field[i] + field[ind] == 10)
      {
        field[i] = 0;
        field[ind] = 0;
        return std::make_tuple(ind, i);
      }
      else
      {
        break;
      }
    }
  }

  // num below
  unsigned downInd = ind + width;
  if (
    downInd < field.size()
    and
    (field[downInd] == field[ind] or field[downInd] + field[ind] == 10)
    )
  {
    field[downInd] = 0;
    field[ind] = 0;
    return std::make_tuple(ind, downInd);
  }

  // num above
  int topInd = ind - width;
  if (
    topInd >= 0
    and
    (field[topInd] == field[ind] or field[topInd] + field[ind] == 10)
    )
  {
    field[topInd] = 0;
    field[ind] = 0;
    return std::make_tuple(ind, topInd);
  }

  return std::make_tuple(ind, 0);
}

bool IsChecking(std::vector<int>& field)
{
  // init checking bool
  bool isChecking = false;

  // find existing num
  unsigned size = field.size();
  for (unsigned i = 0; i < size; i++)
  {
    if (not field[i])
    {
      continue;
    }

    // do check
    field.push_back(field[i]);
    isChecking = true;
  }

  return isChecking;
}


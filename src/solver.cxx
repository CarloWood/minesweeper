#include "sys.h"
#include "Solver.h"
#include "debug.h"

int main()
{
  Debug(NAMESPACE_DEBUG::init());
  Dout(dc::notice, "Entering main()");

  utils::RandomNumber rn; //(0x601f23983f24a);
  Grid grid(rn);

  Vector::print_consts();

  unsigned long attempt = 0;
  for (;;)
  {
    BoolVector possible_first_moves("first moves");
    for (;;)
    {
      possible_first_moves = grid.find_first_moves();
      if (!possible_first_moves.is_empty())
        break;
      grid.randomize(rn);
    }
    Dout(dc::debug, "possible_first_moves = " << possible_first_moves);
    ++attempt;
    int index = possible_first_moves.first_true();
    std::vector<int> possible_moves;
    while (index != -1)
    {
      Dout(dc::debug, "Adding " << index);
      possible_moves.push_back(index);
      index = possible_first_moves.next_true(index);
    }

    //std::cout << grid << std::endl;
    //std::cout << possible_first_moves << std::endl;

    bool solved = true;
    Dout(dc::debug, "Number of possible moves: " << possible_moves.size());
    for (int move : possible_moves)
    {
      Solver solver(&grid);
      solver.click_empty(move);
      //std::cout << solver << std::endl;
      int moves;
      do
      {
        moves = 0;
        do
        {
          BoolVector new_empty = solver.calc_new_empty();
          if (new_empty.is_empty())
            break;
          solver.click_empty(new_empty);
          //std::cout << solver << std::endl;
          ++moves;
        }
        while (1);
        do
        {
          BoolVector new_mines = solver.calc_new_mines();
          if (new_mines.is_empty())
            break;
          solver.click_mines(new_mines);
          //std::cout << solver << std::endl;
          ++moves;
        }
        while (1);
      }
      while (moves);

      if (!solver.solved())
      {
        //std::cout << "Could not entirely solve: " << solver << "\nattempt " << attempt << std::endl;
        if (attempt % 1000 == 0)
          std::cout << "attempt=" << attempt << '\n';
        solved = false;
        //std::cout << solver << '\n';
        break;
      }
    }
    if (solved)
    {
      std::cout << "Could solve this grid without any guessing:" << grid << std::endl;
      break;
    }
    grid.randomize(rn);
  }
}

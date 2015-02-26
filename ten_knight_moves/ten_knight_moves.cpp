// ten_knight_moves.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <cassert>
#include <iostream>
#include <string>
#include <vector>
#include <utility>
#include <map>
#include <algorithm>
#include <fstream>

typedef unsigned char direction;
typedef unsigned char axis;

const axis max_x = 5;
const axis max_y = 4;

char board[max_y][max_x] =
{
   { 'A', 'B', 'C', 'D', 'E' },
   { 'F', 'G', 'H', 'I', 'J' },
   { 'K', 'L', 'M', 'N', 'O' },
   { ' ', '1', '2', '3', ' ' },
};

char const file_name[] = { "strings.txt" };

//struct direction_t
//{
//   direction d;
//   typedef std::vector<direction_t> directions_t;
//   directions_t directions;
//};

typedef std::vector < direction > directions_t;

//struct path_t
//{
//   axis start_x;
//   axis start_y;   
//   //direction_t::directions_t directions;
//   directions_t d;
//};

bool is_valid(axis x, axis y)
{
   if (y < (max_y-1))
      return x < max_x;
   return y == (max_y-1) && (x < 4 && x > 0);
}

/* directions
   | 8.1
   |7 . 2
   |..o..
   |6 . 3
   | 5.4
   +------>
*/

bool is_vowel(char ch)
{
   switch (ch)
   {
      case 'A': return true;
      case 'E': return true;
      case 'I': return true;
      case 'O': return true;
      case 'U': return true;
      default: return false;
   }
   return false;
}


void transform_pos(direction d, axis x, axis y, axis &new_x, axis &new_y)
{
   switch (d)
   {
      case 1: new_x = x + 1; new_y = y - 2; break;
      case 2: new_x = x + 2; new_y = y - 1; break;
      case 3: new_x = x + 2; new_y = y + 1; break;
      case 4: new_x = x + 1; new_y = y + 2; break;
      case 5: new_x = x - 1; new_y = y - 2; break;
      case 6: new_x = x - 2; new_y = y - 1; break;
      case 7: new_x = x - 2; new_y = y + 1; break;
      case 8: new_x = x - 1; new_y = y + 2; break;

      default:new_x = max_x; new_y = max_y; break;
   }
}

bool can_make_step(direction d, axis x, axis y)
{
   assert(is_valid(x, y));
   axis new_x;
   axis new_y;
   transform_pos(d, x, y, new_x, new_y);
   return is_valid( new_x, new_y);
}

static unsigned long total_count = 0;

static axis start_x = 0;
static axis start_y = 0;
static std::ofstream gf;

void dump(directions_t ad)
{   
   std::string s;

   axis x = start_x;
   axis y = start_y;

   for (unsigned i = 0; i < ad.size(); ++i)
   {
      axis new_x;
      axis new_y;
      s += board[y][x];
      transform_pos(ad[i], x, y, new_x, new_y);
      x = new_x;
      y = new_y ;
   }
   if (std::count_if(s.begin(), s.end(), is_vowel) < 3)
   {
      ++total_count;
      assert(gf);
      gf << s << std::endl;      
   }
}

void step(axis x, axis y, directions_t ad )
{
   assert(ad.size() < 10);
   
   for (direction d = 1; d < 9; ++d)
   {
      if (can_make_step(d, x, y))
      {
         directions_t new_ad;
         new_ad = ad;
         axis new_x;
         axis new_y;
         transform_pos(d, x, y, new_x, new_y);
         new_ad.push_back(d);
         //std::cout << new_ad.size()<<std::endl;
         if (new_ad.size() > 9)
         {
            dump(new_ad);
            return;
         }
         else
            step(new_x, new_y, new_ad);
      }
   }   
}

void start(axis x, axis y)
{
   for (direction d = 1; d < 9; ++d)
   {
      if (can_make_step(d, x, y))
      {
         axis new_x;
         axis new_y;
         transform_pos(d, x, y, new_x, new_y);
         
         directions_t ad;

         ad.push_back(d);

         step(new_x, new_y, ad);
      }
   }
}

/*
typedef std::vector<path_t> path_array;

void init(path_array &a)
{
   axis x = 0;
   axis y = 0;

   //for (axis x = 0; x < max_x; ++x)
   //{
      //for (axis y = 0; y < max_y; ++y)
      //{
   if ( is_valid(x, y) )
   {
      path_t p;
      p.start_x = x;
      p.start_y = y;

      for (direction d = 1; d < 9; ++d)
      {
         if (can_make_step(d, x, y) )
         {
            direction_t dir;
            dir.d = d;
            p.directions.push_back( dir );
         }
      }
      a.push_back(p);
   }
   //   }
   //}
}

void next_step(direction_t &dir, axis x, axis y, unsigned char depth)
{

   if (depth > 10)
      return; 

   axis new_x;
   axis new_y;
   transform_pos(dir.d, x, y, new_x, new_y);

   for (direction d = 1; d < 9; ++d)
   {
      if (can_make_step(d, new_x, new_y))
      {
         direction_t new_d;
         new_d.d = d;
         dir.directions.push_back(new_d);

         next_step(dir.directions.back(), new_x, new_y, depth + 1);
      }
   }
}

void step(path_array &a)
{
   path_array::iterator i= a.begin();
   while (i != a.end())
   {
      path_t &p = *i;
      direction_t::directions_t::iterator j = p.directions.begin();
      while (j != p.directions.end())
      {
         direction_t &dir = *j;

         next_step(dir, p.start_x, p.start_y, 0);

         ++j;
      }
      ++i;
   }
}

void next_dump(const direction_t &dir, axis x, axis y, std::string prefix )
{
   axis new_x;
   axis new_y;
   
   direction_t::directions_t::const_iterator j = dir.directions.begin();
   
   prefix += std::string(1, board[y][x]); 

   if (dir.directions.empty())
   {
      std::cout << prefix << '\n';
      return;
   }

   while (j != dir.directions.end())
   {
      const direction_t &new_dir = *j;
      transform_pos(dir.d, x, y, new_x, new_y);

      next_dump(new_dir, new_x, new_y, prefix );
   }
}


void dump(const path_t &path)
{
   std::string prefix;

   direction_t::directions_t::const_iterator j = path.directions.begin();
   while (j != path.directions.end())
   {
      const direction_t &dir = *j;
      next_dump(dir, path.start_x, path.start_y, prefix );
      ++j;
   }

}
*/

int _tmain(int argc, _TCHAR* argv[])
{   
   total_count = 0;

   gf.open(file_name);

   for (axis x = 0; x < max_x; ++x)
   {
      for (axis y = 0; y < max_y; ++y)
      {
         if (is_valid(x, y))
         {
            start_x = x;
            start_y = y;

            start(x, y);
         }
      }
   }

   gf.flush();
   gf.close();

   std::cout << total_count << std::endl << std::endl;

   std::ifstream f(file_name);
   while (f)
   {
      std::string s;
      std::getline(f, s);
      if (!s.empty())
      {
         std::cout << s << std::endl;
      }
   }
   f.close();

   return 0;
}


#ifndef __ONE_TO_ONE_ONTO_HPP__
#define __ONE_TO_ONE_ONTO_HPP__
#include <map>

template <class Left, class Right>
class OnetoOne_onto {
 private:
  std::map<Left, Right> LtoR;
  std::map<Right, Left> RtoL;
  size_t otoot_size;

 public:
  OnetoOne_onto() { otoot_size = 0; }
  ~OnetoOne_onto() {}
  void insert(Left l, Right r) {
    LtoR[l] = r;
    RtoL[r] = l;
    otoot_size++;
  }
  bool hasL(Left l) { return (LtoR.find(l) != LtoR.end()); }
  bool hasR(Right r) { return (RtoL.find(r) != RtoL.end()); }
  Left getL(Right r) { return RtoL[r]; }
  Right getR(Left l) { return LtoR[l]; }
  size_t size() { return otoot_size; }
};

#endif
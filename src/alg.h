#ifndef _ALG_H_
#define _ALG_H_

#include <bits/stdc++.h>
using namespace std;

class NODE
{
public:
    double llx, lly, width, height;
    bool fixed = false;
};
class NET
{
public:
    vector<NODE *> nodes;
    vector<pair<double ,double>> pins;
};
class EDGE
{
public:
    double supply = 0, demand = 0;

    EDGE(double supply) : supply(supply) {}
};
class GCELL
{
public:
    double overflow = 0;
};
class WIRE
{
public:
    double coord, front, back;
    bool half;

    WIRE(double coord, double front, double back, bool half)
     : coord(coord), front(front), back(back), half(half) {}
};
class ALG
{
public:
    void SetLayerSupply();
    void SetSupply();
    void CreatWire(int, int, int, int);
    void SetDemand();
    void SetOverFlow();
    double GetTenPercent(vector<vector<GCELL *>> *);
    
    vector<NODE *> node_vec;
    vector<NET *> net_vec;
    size_t layer, ver_num, hor_num;
    vector<size_t> layer_blocked_vec = {0, 1, 2, 3};
    vector<vector<EDGE *>> ver_vec, hor_vec;
    vector<double> layer_width_vec;
    vector<double> layer_spacing_vec;
    vector<double> layer_supply_ver_vec;
    vector<double> layer_supply_hor_vec;
    double ver_ofs, hor_ofs;
    double tile_h, tile_w;
    vector<WIRE *> ver_wire_vec, hor_wire_vec;
    vector<vector<GCELL *>> ver_cell_vec, hor_cell_vec;

private:
    
};

#endif
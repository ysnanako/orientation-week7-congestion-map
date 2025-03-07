#include "alg.h"
using namespace std;

void ALG::SetLayerSupply()
{
    cout << "layer: " << layer << "\n";
    cout << "tile_w: " << tile_w << "\n";
    cout << "tile_h: " << tile_h << "\n";
    for(size_t i = 0; i < layer; ++i)
    {
        layer_supply_ver_vec.push_back(tile_h / (layer_width_vec[i] + layer_spacing_vec[i]));
        layer_supply_hor_vec.push_back(tile_w / (layer_width_vec[i] + layer_spacing_vec[i]));
    }
    // int index = 0;
    // for(auto const &supply : layer_supply_ver_vec)
    // {
    //     cout << "layer_ver" << index << " " << supply << "\n";
    //     index++;
    // }
    // index = 0;
    // for(auto const &supply : layer_supply_hor_vec)
    // {
    //     cout << "layer_hor" << index << " " << supply << "\n";
    //     index++;
    // }
}
void ALG::SetSupply()
{
    // intitialize
    double total_layer_supply_ver = 0;
    double total_layer_supply_hor = 0;
    for(size_t i = 0; i < layer; ++i)
    {
        total_layer_supply_ver += layer_supply_ver_vec[i];
        total_layer_supply_hor += layer_supply_hor_vec[i];
    }
    ver_vec.resize(hor_num + 1);
    hor_vec.resize(ver_num + 1);
    for(auto &ver : ver_vec)
    {
        for(size_t i = 0; i < ver_num; ++i)
        {
            EDGE *new_ptr = new EDGE(total_layer_supply_ver);
            ver.push_back(new_ptr);
        }
    }
    for(auto &hor : hor_vec)
    {
        for(size_t i = 0; i < hor_num; ++i)
        {
            EDGE *new_ptr = new EDGE(total_layer_supply_hor);
            hor.push_back(new_ptr);
        }
    }
    // blockage
    for(auto const &node : node_vec)
    {
        if(!node->fixed)
            continue;
        // set index vec
        vector<size_t> index_x_vec;
        vector<size_t> index_y_vec;
        size_t front_index;
        size_t back_index;
        // x
        front_index = (node->llx - ver_ofs) / tile_w;
        back_index = (node->llx + node->width - ver_ofs) / tile_w;
        for(size_t i = 0; i <= back_index - front_index; ++i)
        {
            index_x_vec.push_back(front_index + i);
        }
        // y
        front_index = (node->lly - hor_ofs) / tile_h;
        back_index = (node->lly + node->height - hor_ofs) / tile_h;
        for(size_t i = 0; i <= back_index - front_index; ++i)
        {
            index_y_vec.push_back(front_index + i);
        }
        // update supply
        // ver
        for(size_t i = 1; i < index_x_vec.size(); ++i)
        {
            for(auto const &lb : layer_blocked_vec)
            {
                for(size_t j = 0; j < index_y_vec.size(); ++j)
                {
                    ver_vec[index_x_vec[i]][index_y_vec[j]]->supply -= layer_supply_ver_vec[lb];
                }
                ver_vec[index_x_vec[i]][index_y_vec.front()]->supply += (node->lly - index_y_vec.front() * tile_h - hor_ofs) / (layer_width_vec[lb] + layer_spacing_vec[lb]);
                ver_vec[index_x_vec[i]][index_y_vec.back()]->supply += ((index_y_vec.back() + 1) * tile_h + hor_ofs - (node->lly + node->height)) / (layer_width_vec[lb] + layer_spacing_vec[lb]);
            }
        }
        // hor
        for(size_t i = 1; i < index_y_vec.size(); ++i)
        {
            for(auto const &lb : layer_blocked_vec)
            {
                for(size_t j = 0; j < index_x_vec.size(); ++j)
                {
                    hor_vec[index_y_vec[i]][index_x_vec[j]]->supply -= layer_supply_hor_vec[lb];
                }
                hor_vec[index_y_vec[i]][index_x_vec.front()]->supply += (node->llx - index_x_vec.front() * tile_w - ver_ofs) / (layer_width_vec[lb] + layer_spacing_vec[lb]);
                hor_vec[index_y_vec[i]][index_x_vec.back()]->supply += ((index_x_vec.back() + 1) * tile_w + ver_ofs - (node->llx + node->width)) / (layer_width_vec[lb] + layer_spacing_vec[lb]);
            }
        }
    }
}
void ALG::CreatWire(int x1, int y1, int x2, int y2)
{
    if(x1 == x2)
    {
        WIRE *new_ptr = new WIRE((double)x1, (double)min(y1, y2), (double)max(y1, y2), false);
        ver_wire_vec.push_back(new_ptr);
        return;
    }
    if(y1 == y2)
    {
        WIRE *new_ptr = new WIRE((double)y1, (double)min(x1, x2), (double)max(x1, x2), false);
        hor_wire_vec.push_back(new_ptr);
        return;
    }
    WIRE *new_ptr;
    new_ptr = new WIRE((double)x1, (double)min(y1, y2), (double)max(y1, y2), true);
    ver_wire_vec.push_back(new_ptr);
    new_ptr = new WIRE((double)x2, (double)min(y1, y2), (double)max(y1, y2), true);
    ver_wire_vec.push_back(new_ptr);
    new_ptr = new WIRE((double)y1, (double)min(x1, x2), (double)max(x1, x2), true);
    hor_wire_vec.push_back(new_ptr);
    new_ptr = new WIRE((double)y2, (double)min(x1, x2), (double)max(x1, x2), true);
    hor_wire_vec.push_back(new_ptr);
    return;
}
void ALG::SetDemand()
{
    for(auto const &ver_wire : ver_wire_vec)
    {
        vector<size_t> index_y_vec;
        size_t front_index;
        size_t back_index;
        front_index = (ver_wire->front - ver_ofs) / tile_h;
        back_index = (ver_wire->back - ver_ofs) / tile_h;
        for(size_t i = 0; i <= back_index - front_index; ++i)
        {
            index_y_vec.push_back(front_index + i);
        }
        for(size_t i = 1; i < index_y_vec.size(); ++i)
        {
            if(ver_wire->half) 
                hor_vec[i][(ver_wire->coord - ver_ofs) / tile_w]->demand += 0.5;
            else
                hor_vec[i][(ver_wire->coord - ver_ofs) / tile_w]->demand += 1;
        }
    }
    for(auto const &hor_wire : hor_wire_vec)
    {
        vector<size_t> index_x_vec;
        size_t front_index;
        size_t back_index;
        front_index = (hor_wire->front - hor_ofs) / tile_w;
        back_index = (hor_wire->back - hor_ofs) / tile_w;
        for(size_t i = 0; i <= back_index - front_index; ++i)
        {
            index_x_vec.push_back(front_index + i);
        }
        for(size_t i = 1; i < index_x_vec.size(); ++i)
        {
            if(hor_wire->half) 
                ver_vec[i][(hor_wire->coord - hor_ofs) / tile_h]->demand += 0.5;
            else
                ver_vec[i][(hor_wire->coord - hor_ofs) / tile_h]->demand += 1;
        }
    }
}
void ALG::SetOverFlow()
{
    ver_cell_vec.resize(hor_num);
    for(size_t i = 0; i < hor_num; ++i)
    {
        for(size_t j = 0; j < ver_num; ++j)
        {
            GCELL *new_ptr = new GCELL;
            new_ptr->overflow = max((ver_vec[i][j]->demand - ver_vec[i][j]->supply), 
            (ver_vec[i + 1][j]->demand - ver_vec[i + 1][j]->supply));
            ver_cell_vec[i].push_back(new_ptr);
        }
    }
    hor_cell_vec.resize(ver_num);
    for(size_t i = 0; i < ver_num; ++i)
    {
        for(size_t j = 0; j < hor_num; ++j)
        {
            GCELL *new_ptr = new GCELL;            
            new_ptr->overflow = max((hor_vec[i][j]->demand - hor_vec[i][j]->supply), 
            (hor_vec[i + 1][j]->demand - hor_vec[i + 1][j]->supply));
            hor_cell_vec[i].push_back(new_ptr);
        }
    }
}
double ALG::GetTenPercent(vector<vector<GCELL *>> *cell_vec)
{
    size_t ten_percent_num = (size_t)(ver_num * hor_num * 0.1);
    priority_queue<double, vector<double>, greater<double>> heap;
    heap.push(numeric_limits<double>::lowest());
    for(auto const &cell : *cell_vec)
    {
        for(auto const &c : cell)
        {
            if(heap.size() < ten_percent_num)
            {
                heap.push(c->overflow);
                continue;
            }
            if(heap.top() < c->overflow)
            {
                heap.pop();
                heap.push(c->overflow);
            }
        }
    }
    return heap.top();
}
#include "alg.h"
#include "parser.h"
using namespace std;

extern "C"{
    #include "flute.h"
}

int main(int argc, char *argv[])
{
    PARSER PARSER_obj;
    ALG ALG_obj;
    
    PARSER_obj.GetFilename(argc, argv);
    PARSER_obj.ReadAuxFile(ALG_obj);
    PARSER_obj.ReadNodeFile(ALG_obj);
    PARSER_obj.ReadPlFile(ALG_obj);
    PARSER_obj.ReadNetFile(ALG_obj);
    PARSER_obj.ReadRouteFile(ALG_obj);

    readLUT();
    int index = 0;
    for(const auto &net : ALG_obj.net_vec)
    {
        if(net->pins.size() < 2)
            continue;
        vector<int> x;
        vector<int> y;
        for(const auto &pin : net->pins)
        {
            x.push_back((int)pin.first);
            y.push_back((int)pin.second);
        }
        Tree ft = flute(x.size(), x.data(), y.data(), ACCURACY);
        cout << "net" << index;
        cout << " (" << wirelength(ft) << ")\n";
        // printtree(ft);

        for(int i = 0; i < ft.deg; ++i)
        {
            int j = ft.branch[i].n;
            ALG_obj.CreatWire(ft.branch[i].x, ft.branch[i].y, ft.branch[j].x, ft.branch[j].y);
        }

        // COUT LINE SEGMENT
        // for(int i = 0; i < ft.deg; ++i)
        // {
        //     int x1 = ft.branch[i].x;
        //     int y1 = ft.branch[i].y;
        //     int j = ft.branch[i].n;
        //     int x2 = ft.branch[j].x;
        //     int y2 = ft.branch[j].y;
        //     cout << "Line segment from (" << x1 << ", " << y1 << ") to ("
        //         << x2 << ", " << y2 << ")\n";
        // }
    
        index++;
    }
    ALG_obj.SetLayerSupply();
    ALG_obj.SetSupply();
    ALG_obj.SetDemand();
    for(auto const &ver : ALG_obj.ver_vec)
    {
        for(auto const &e : ver)
        {
            if(e->demand != 0)   
                cout << "ver supply: " << e->supply << " demand: " << e->demand << "\n";
        }
        cout << "-----------next-----------\n";
    }
    for(auto const &hor : ALG_obj.hor_vec)
    {
        for(auto const &e : hor)
        {
            if(e->demand != 0)
                cout << "hor supply: " << e->supply << " demand: " << e->demand << "\n";
        }
        cout << "-----------next-----------\n";
    }
    ALG_obj.SetOverFlow();
    for(auto const &ver_cell : ALG_obj.ver_cell_vec)
    {
        for(auto const &cell : ver_cell)
        {
            if(cell->overflow > -96)
                cout << "ver cell overflow: " << cell->overflow << "\n";
        }
        cout << "-----------next-----------\n";
    }
    for(auto const &hor_cell : ALG_obj.hor_cell_vec)
    {
        for(auto const &cell : hor_cell)
        {
            if(cell->overflow > -96)
                cout << "hor cell overflow: " << cell->overflow << "\n";
        }
        cout << "-----------next-----------\n";
    }
    // COUT WIRE
    // for(auto const &ver_wire : ALG_obj.ver_wire_vec)
    // {
    //     cout << "ver x: " << ver_wire->coord << " y: " << ver_wire->front << "->" << ver_wire->back;
    //     cout << " (" << ver_wire->half << ")\n";
    // }
    // for(auto const &hor_wire : ALG_obj.hor_wire_vec)
    // {
    //     cout << "hor y: " << hor_wire->coord << " x: " << hor_wire->front << "->" << hor_wire->back;
    //     cout << " (" << hor_wire->half << ")\n";
    // }

    // EXAMPLE
    // readLUT();
    // int x[3] = {1, 2, 3};
    // int y[3] = {1, 0, 3};
    // Tree ft = flute(3, x, y, ACCURACY);
    // cout << wirelength(ft) << endl;
    // printtree(ft);

    PARSER_obj.WriteFile(ALG_obj);

    return EXIT_SUCCESS; 
}

// ./FLUTE_test adaptec1.aux adaptec1_ver.dpx adaptec1_hor.dpx adaptec1_w_ver.dpx adaptec1_w_hor.dpx
// ./FLUTE_test adaptec2.aux adaptec2_ver.dpx adaptec2_hor.dpx adaptec2_w_ver.dpx adaptec2_w_hor.dpx
// ./FLUTE_test adaptec3.aux adaptec3_ver.dpx adaptec3_hor.dpx adaptec3_w_ver.dpx adaptec3_w_hor.dpx

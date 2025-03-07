#include "parser.h"
using namespace std;

void PARSER::GetFilename(const int& argc, char *argv[])
{
    if(argc == 6)
    {
        this->aux_filename = argv[1];
        this->ver_filename = argv[2];
        this->hor_filename = argv[3];
        this->w_ver_filename = argv[4];
        this->w_hor_filename = argv[5];
    }
    else
    {
        cerr << "Format: ./exe <circuit.aux> <circuit.dpx> <circuit.dpx>. \n";
        exit(EXIT_FAILURE);
    }
}
void PARSER::ReadAuxFile(ALG &ALG_obj)
{
    fin.open(aux_filename);
    if(!fin)
    {
        cerr << "Error: Input .aux file failed. \n";
        exit(EXIT_FAILURE);
    }
    string temp;
    fin >> temp >> temp >> node_filename >> net_filename;
    fin >> temp >> pl_filename >> scl_filename >> temp >> route_filename;
    fin.close();
}
void PARSER::ReadNodeFile(ALG &ALG_obj)
{
    fin.open(node_filename);
    if(!fin)
    {
        cerr << "Error: Input .nodes file failed. \n";
        exit(EXIT_FAILURE);
    }
    string line;
    while(getline(fin, line))
    {
        if(line.size() < 13)
            continue;
        if(line.substr(0, 5) == "    o" || line.substr(0, 4) == "\t\t\to")
            CreatNode(ALG_obj, line);
    }
    fin.close();
}
void PARSER::CreatNode(ALG &ALG_obj, string line)
{
    stringstream ss(line);
    string s;
    ss >> s >> s;
    NODE *new_ptr = new NODE;
    new_ptr->width = stod(s);
    ss >> s;
    new_ptr->height = stod(s);
    if(line.back() == 'l') // terminal node
        new_ptr->fixed = true;
    ALG_obj.node_vec.push_back(new_ptr);
}
void PARSER::ReadNetFile(ALG &ALG_obj)
{
    fin.open(net_filename);
    if(!fin)
    {
        cerr << "Error: Input .nets file failed. \n";
        exit(EXIT_FAILURE);
    }
    string line;
    while(getline(fin, line))
    {
        if(line.substr(0, 9) == "NetDegree")
            ALG_obj.net_vec.push_back(new NET);
        else if(line.substr(0, 2) == "\to")
            CreatNet(ALG_obj, line);
    }
    fin.close();
}
void PARSER::CreatNet(ALG &ALG_obj, string line)
{
    stringstream ss(line);
    string s;
    double x, y;
    ss >> s;
    NODE *node_ptr = ALG_obj.node_vec[stoul(s.erase(0, 1))];
    ss >> s >> s >> s;
    x = node_ptr->llx + node_ptr->width / 2 + stod(s);
    ss >> s;
    y = node_ptr->lly + node_ptr->height / 2 + stod(s);
    ALG_obj.net_vec.back()->nodes.push_back(node_ptr);
    ALG_obj.net_vec.back()->pins.push_back(make_pair(x, y));
}
void PARSER::ReadPlFile(ALG &ALG_obj)
{
    fin.open(pl_filename);
    if(!fin)
    {
        cerr << "Error: Input .pl file failed. \n";
        exit(EXIT_FAILURE);
    }
    string line;
    while(getline(fin, line))
    {
        if(line.substr(0, 1) == "o")
            SetNodePos(ALG_obj, line);
    }
    fin.close();
}
void PARSER::SetNodePos(ALG &ALG_obj, string line)
{
    stringstream ss(line);
    string s;
    ss >> s;
    NODE *node_ptr = ALG_obj.node_vec[stoul(s.erase(0, 1))];
    ss >> s;
    node_ptr->llx = stod(s);
    ss >> s;
    node_ptr->lly = stod(s);
}
void PARSER::ReadSclFile(ALG &ALG_obj)
{

}
void PARSER::ReadRouteFile(ALG &ALG_obj)
{
    fin.open(route_filename);
    if(!fin)
    {
        cerr << "Error: Input .route file failed. \n";
        exit(EXIT_FAILURE);
    }
    string line;
    while(getline(fin, line))
    {
        if(line.size() < 14)
            continue;
        else if(line.substr(0, 8) == "Grid    ")
        {
            stringstream ss(line);
            string s;
            ss >> s >> s >> s;
            ALG_obj.hor_num = stoul(s);
            ss >> s;
            ALG_obj.ver_num = stoul(s);
            ss >> s;
            ALG_obj.layer = stoul(s);
        }
        else if(line.substr(0, 12) == "MinWireWidth")
        {
            stringstream ss(line);
            string s;
            ss >> s >> s;
            while(ss >> s)
                ALG_obj.layer_width_vec.push_back(stod(s));
        }
        else if(line.substr(0, 14) == "MinWireSpacing")
        {
            stringstream ss(line);
            string s;
            ss >> s >> s;
            while(ss >> s)
                ALG_obj.layer_spacing_vec.push_back(stod(s));
        }
        else if(line.substr(0, 10) == "GridOrigin")
        {
            stringstream ss(line);
            string s;
            ss >> s >> s >> s;
            ALG_obj.ver_ofs = stod(s);
            ss >> s;
            ALG_obj.hor_ofs = stod(s);
        }
        else if(line.substr(0, 8) == "TileSize")
        {
            stringstream ss(line);
            string s;
            ss >> s >> s >> s;
            ALG_obj.tile_w = stod(s);
            ss >> s;
            ALG_obj.tile_h = stod(s);
            break;
        }
    }
    fin.close();
}
void PARSER::WriteFile(ALG &ALG_obj)
{
    fout.open(ver_filename);
    if(!fout)
    {
        cerr << "Error: Output ver.dpx file failed. \n";
        exit(EXIT_FAILURE);
    }
    double ver_ten_percent = ALG_obj.GetTenPercent(&ALG_obj.ver_cell_vec);
    for(size_t i = 0; i < ALG_obj.ver_cell_vec.size(); ++i)
    {
        for(size_t j = 0; j < ALG_obj.ver_cell_vec[i].size(); ++j)
        {
            if(ver_ten_percent < ALG_obj.ver_cell_vec[i][j]->overflow)
                fout << "COLOR pink\n";
            else
                fout << "COLOR blue\n";
            fout << "SRF " << i * ALG_obj.tile_w << " " << -(j * ALG_obj.tile_h) << " ";
            fout << (i + 1) * ALG_obj.tile_w << " " << -((j + 1) * ALG_obj.tile_h) << "\n";
        }
    }
    fout.close();

    fout.open(hor_filename);
    if(!fout)
    {
        cerr << "Error: Output hor.dpx file failed. \n";
        exit(EXIT_FAILURE);
    }
    double hor_ten_percent = ALG_obj.GetTenPercent(&ALG_obj.hor_cell_vec);
    for(size_t i = 0; i < ALG_obj.hor_cell_vec.size(); ++i)
    {
        for(size_t j = 0; j < ALG_obj.hor_cell_vec[i].size(); ++j)
        {
            if(hor_ten_percent < ALG_obj.hor_cell_vec[i][j]->overflow)
                fout << "COLOR green\n";
            else
                fout << "COLOR blue\n";
            fout << "SRF " << j * ALG_obj.tile_w << " " << -(i * ALG_obj.tile_h) << " ";
            fout << (j + 1) * ALG_obj.tile_w << " " << -((i + 1) * ALG_obj.tile_h) << "\n";
        }
    }
    fout.close();

    cout << "ver_ten_percent: " << ver_ten_percent << "\n";
    cout << "hor_ten_percent: " << hor_ten_percent << "\n";

    fout.open(w_ver_filename);
    if(!fout)
    {
        cerr << "Error: Output w_ver.dpx file failed. \n";
        exit(EXIT_FAILURE);
    }
    for(auto const &w : ALG_obj.ver_wire_vec)
    {
        fout << "COLOR red\n";
        fout << "SL " << w->coord << " " << -w->front << " ";
        fout << w->coord << " " << -w->back << "\n";
    }
    for(size_t i = 0; i < ALG_obj.ver_cell_vec.size(); ++i)
    {
        for(size_t j = 0; j < ALG_obj.ver_cell_vec[i].size(); ++j)
        {
            
            fout << "COLOR gray\n";
            fout << "SR " << i * ALG_obj.tile_w << " " << -(j * ALG_obj.tile_h) << " ";
            fout << (i + 1) * ALG_obj.tile_w << " " << -((j + 1) * ALG_obj.tile_h) << "\n";
        }
    }
    fout.close();

    fout.open(w_hor_filename);
    if(!fout)
    {
        cerr << "Error: Output w_hor.dpx file failed. \n";
        exit(EXIT_FAILURE);
    }
    for(auto const &w : ALG_obj.hor_wire_vec)
    {
        fout << "COLOR red\n";
        fout << "SL " << w->front << " " << -w->coord << " ";
        fout << w->back << " " << -w->coord << "\n";
    }
    for(size_t i = 0; i < ALG_obj.hor_cell_vec.size(); ++i)
    {
        for(size_t j = 0; j < ALG_obj.hor_cell_vec[i].size(); ++j)
        {
            fout << "COLOR gray\n";
            fout << "SR " << j * ALG_obj.tile_w << " " << -(i * ALG_obj.tile_h) << " ";
            fout << (j + 1) * ALG_obj.tile_w << " " << -((i + 1) * ALG_obj.tile_h) << "\n";
        }
    }
    fout.close();
}
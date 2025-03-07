#ifndef _PARSER_H_
#define _PARSER_H_

#include <bits/stdc++.h>
#include "alg.h"
using namespace std;

class PARSER
{
    public:
    string aux_filename, ver_filename, hor_filename, w_ver_filename, w_hor_filename;
    string node_filename, net_filename, pl_filename, scl_filename, route_filename;
    void GetFilename(const int& argc, char *argv[]);
    void ReadAuxFile(ALG &);
    void ReadNodeFile(ALG &);
    void ReadNetFile(ALG &);
    void ReadPlFile(ALG &);
    void ReadSclFile(ALG &);
    void ReadRouteFile(ALG &);
    void WriteFile(ALG &);
    private:
    ifstream fin;
    ofstream fout;
    void CreatNode(ALG &, string);
    void CreatNet(ALG &, string);
    void SetNodePos(ALG &, string);
};

#endif
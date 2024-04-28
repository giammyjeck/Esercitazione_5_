#include <iostream>
#include "PolygonalMesh.hpp"
#include "Utils.hpp"

/// TO DO:
/// verificare che numero lati e numero archi della cella 2d siano gli stessi
/// precisione
/// meglio vector<vector<unsigned int>> opp vector<VectorXi>> ???
/// cancellare numEdges e numVertices se non servono

using namespace std;
using namespace Eigen;
using namespace MyPolygonalMeshLibrary;

int main()
{
    double eps = 10e-7; // definisco la tolleranza per il progetto: tolleranza per la distanza fra punti
    PolygonalMesh mesh; //definisco una struttura PolygonalMesh


    string filepath = "PolygonalMesh";
    if(!ImportMesh(filepath, mesh, eps))
    {
        cerr << "something went wrong"<< endl;
        return 1;
    }



  return 0;
}




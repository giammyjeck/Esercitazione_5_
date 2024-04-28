#pragma once

#include <iostream>
#include "Eigen/Eigen" //Vector2d

using namespace std;  // vector,list, map
using namespace Eigen;


namespace MyPolygonalMeshLibrary {

struct PolygonalMesh
{
    ///celle 0D [id marker X Y ]
    // memorizzo il numero di celle
    // creo un vector di interi dove memorizzo gli Id delle celle
    // creo un vector di vettori a due componenti in cui salvo le coordinate (X,Y) delle celle
    // creo un dizionario per salvare i marker, ad ogni marker associo una lista con gli Id delle celle che hanno quel marker
    //
    unsigned int NumberCell0D = 0; ///< number of Cell0D
    vector<unsigned int> Cell0DId = {}; ///< Cell0D id, size 1 x NumberCell0D
    vector<Vector2d> Cell0DCoordinates = {}; ///< Cell0D coordinates, size 2 x NumberCell0D (x,y)
    map<unsigned int, list<unsigned int>> Cell0DMarkers = {}; ///< Cell0D markers, size 1 x NumberCell0D (marker)

    ///celle 1D [id marker origin end ]
    // memorizzo il numero di celle
    // creo un vector di interi dove memorizzo gli Id delle celle
    // creo un vector di vettori a due componenti in cui salvo l'id di origine e fine delle celle0D che formano la cella1D
    // creo un dizionario per salvare i marker, ad ogni marker associo una lista con gli Id delle celle che hanno quel marker
    //
    unsigned int NumberCell1D = 0; ///< number of Cell1D
    vector<unsigned int> Cell1DId = {}; ///< Cell1D id, size 1 x NumberCell1D
    vector<Vector2i> Cell1DVertices = {}; ///< Cell1D vertices indices, size 2 x NumberCell1D (fromId,toId)
    map<unsigned int, list<unsigned int>> Cell1DMarkers = {}; ///< Cell1D markers, size 1 x NumberCell1D (marker)


    ///celle 2D [id marker numvertices vertices numedges edges ]
    // memorizzo il numero di celle
    // creo un vector di interi dove memorizzo gli Id delle celle
    // creo un vector di Vector per memorizzare la sequenza dei vertici della cella (i vertici sono gli id delle celle 0D)
    // creo un vector di Vector per memorizzare la sequenza degli archi della cella (gli archi sono gli id delle celle 1D)
    //
    unsigned int NumberCell2D = 0; ///< number of Cell2D
    vector<unsigned int> Cell2DId = {}; ///< Cell2D id, size 1 x NumberCell2D
    vector<VectorXi> Cell2DVertices = {}; ///< Cell2D Vertices indices
    vector<VectorXi> Cell2DEdges = {}; ///< Cell2D Cell1D indices



};

}


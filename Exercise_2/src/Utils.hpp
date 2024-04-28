#pragma once
#include <iostream>
#include "PolygonalMesh.hpp"

using namespace std;
namespace MyPolygonalMeshLibrary{

/// Importa la mesh poligonale e testa se è costruita correttamente
// filepath: è la cartella dove sono contenuti i file per costruire la mesh
// mesh: struttura di tipo PolygonalMesh
// riempie la mesh con i dati che legge dai file quindi deve poter modificare la mesh in input
// per far ciò chiama le funzioni ImportCell0Ds, ImportCell1Ds, ImportCell2Ds
// restituisce true se è andato tutto a buon fine
bool ImportMesh(const string &filepath,
                PolygonalMesh& mesh,
                const double &eps);

/// Importa le Cell0D
// filename: è il file .csv che contiene le informazioni sulle celle
// mesh: struttura di tipo PolygonalMesh
// restituisce true se è andato tutto a buon fine
bool ImportCell0Ds(const string &filename,
                   PolygonalMesh& mesh);

/// Importa le Cell1D
// filename: è il file .csv che contiene le informazioni sulle celle
// mesh: struttura di tipo PolygonalMesh
// restituisce true se è andato tutto a buon fine
bool ImportCell1Ds(const string &filename,
                   PolygonalMesh& mesh);

/// Importa le Cell2D
// filename: è il file .csv che contiene le informazioni sulle celle
// mesh: struttura di tipo PolygonalMesh
// restituisce true se è andato tutto a buon fine
bool ImportCell2Ds(const string &filename,
                   PolygonalMesh &mesh);

/// Funzione che gestisce i test per le Celle2D
// chiama una serie di funzioniTest e ne gestisce gli output
// chiama Test1Cell2Ds; Test2Cell2Ds;
// mesh: struttura di tipo PolygonalMesh
// restituisce true se tutto è andato a buon fine e le Celle2D hanno superato tutti i test
bool TestMasterCell2Ds(const PolygonalMesh &mesh, const double &eps);

/// Funzione che controlla se gli estremi di ogni arco compaiono nella lista vertici
// mesh: struttura di tipo PolygonalMesh
// restituisce true se gli archi e gli estremi combaciano
bool Test1Cell2Ds(const PolygonalMesh &mesh);

/// Funzione che controlla se gli archi hanno lunghezza nulla
// mesh: struttura di tipo PolygonalMesh
// eps: è la minima distanza fra due punti che si vuol percepire
// restituisce true se gli archi hanno lunghezza non nulla
bool Test2Cell2Ds(const PolygonalMesh &mesh, const double &eps);


/// Funzione che controlla se i poligoni hanno area nulla
// mesh: struttura di tipo PolygonalMesh
// eps: è la minima distanza fra due punti che si vuol percepire
// restituisce true se le aree sono diverse da zero
bool Test3Cell2Ds(const PolygonalMesh &mesh, const double &eps);




}

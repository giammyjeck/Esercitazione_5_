#include "Utils.hpp"
#include <iostream>
#include <fstream>
#include <sstream>

namespace MyPolygonalMeshLibrary{

bool ImportMesh(const string &filepath,
                PolygonalMesh& mesh,
                const double &eps)
{
    /// Importa le celle0D
    // 1) chiama ImportCell0Ds
    // 2) se non fallisce stampo per ogni marker gli id delle celle corrispondenti
    if(!ImportCell0Ds(filepath + "/Cell0Ds.csv", mesh))
    {
        cerr << "errore di importazione celle0D" << endl;
        return false;
    }
    else
    {
        // controllo che le celle 0D siano corrette visivamente,
        // per ogni marker stampo l'id delle celle corrispondenti
        cout << "*** Cell0D MARKER ***" << endl;
        for(auto iter = mesh.Cell0DMarkers.begin(); iter != mesh.Cell0DMarkers.end(); iter++)
        {
            cout << "Marker: " << iter -> first << " Id:";
            for(const unsigned int id : iter -> second)
                cout << " " << id;

            cout << endl;
        }
        cout << endl;
    }

    /// Importa le celle1D
    // chiama ImportCell1Ds
    // se non fallisce stampo per ogni marker gli id corrispondenti
    if(!ImportCell1Ds(filepath + "/Cell1Ds.csv", mesh))
    {
        cerr << "errore di importazione celle1D" << endl;
        return false;
    }
    else
    {
        cout << " *** Cell1D MARKER ***" << endl;
        for(auto iter = mesh.Cell1DMarkers.begin(); iter != mesh.Cell1DMarkers.end(); iter++)
        {
            cout << "Marker: " << iter -> first << " Id:";
            for(const unsigned int id : iter -> second)
                cout << " " << id;

            cout << endl;
        }
        cout << endl;
    }

    /// Importa le celle2D
    // chiama ImportCell2Ds
    // se non fallisce chiama la funzione TestMasterCell2Ds per fare i test sulle celle
    if(!ImportCell2Ds(filepath + "/Cell2Ds.csv", mesh))
    {
        cerr << "errore importazione celle2D" << endl;
        return false;
    }
    else
    {
        bool resultTest =  TestMasterCell2Ds(mesh, eps);
        if (resultTest == false)
        {
            cerr << "la mesh ha fallito qualche test celle2Ds" << endl;
            return false;
        }
    }



    return true;
}

///*****************************************

bool ImportCell0Ds(const string &filename, PolygonalMesh& mesh){

    /// Apro il file
    ifstream file;
    file.open(filename);

    if(file.fail())
    {
        cerr << "impossibile aprire il file: "<< filename << endl;
        return false;
    }

    /// Voglio una lista in cui i nodi sono le info delle celle0D
    // creo una lista di stringhe
    // finchè non finisce il file: prendo una riga del file,
    // la metto nella variabile str line, l'aggiungo in fondo alla lista.
    list<string> listLines;
    string line;
    while (getline(file, line))
        listLines.push_back(line);

    file.close();

    // mi ritrovo con una lista di stringhe che dovrò convertire,
    //intanto scarto la prima riga che è l'intestazione
    listLines.pop_front();

    /// Gestione della memoria
    // aggiorno il NumberCell0D della mesh
    mesh.NumberCell0D = listLines.size();

    if (mesh.NumberCell0D == 0)
    {
        cerr << "There is no cell 0D" << endl;
        return false;
    }

    // ora che conosco la dimensione predispongo lo spazio necessario per riempire i miei vector
    mesh.Cell0DId.reserve(mesh.NumberCell0D);
    mesh.Cell0DCoordinates.reserve(mesh.NumberCell0D);


    /// Passo le info dalla lista alla struct mesh
    // ciclo sui nodi della lista,
    // ogni nodo (cella) lo seziono e inserisco le singole parti al loro posto
    for (const string& line : listLines)
    {
        istringstream converter(line);

        // variabili di "dissezione"
        unsigned int id;
        unsigned int marker;
        Vector2d coord;
        char ignore;

        converter >> id >> ignore >> marker >> ignore >> coord(0) >> ignore>> coord(1);

        mesh.Cell0DId.push_back(id);
        mesh.Cell0DCoordinates.push_back(coord);

        // gestione marker:
        // 1) se la cella non è sul bordo memorizzo marker e Id
        // 2) aggiungo al dizionario la coppia {marker, {id}} dove {id} è una lista di Id
        // 3) se ho già una chiave per quel marker, aggiungo solo l'Id alla lista
        if( marker != 0)
        {
            auto ret = mesh.Cell0DMarkers.insert({marker, {id}});

            if(!ret.second)
                (ret.first)->second.push_back(id);
        }
    }

    cout << "celle 0D importate correttamente" << endl;
    return true;
}

///*****************************************

bool ImportCell1Ds(const string &filename, PolygonalMesh& mesh){

    /// Apro il file
    ifstream file;
    file.open(filename);

    if(file.fail())
    {
        cerr << "impossibile aprire il file: "<< filename << endl;
        return false;
    }

    /// Voglio una lista in cui i nodi sono le info delle celle1D
    list<string> listLines;
    string line;
    while (getline(file, line))
        listLines.push_back(line);

    file.close();

    listLines.pop_front();

    /// Gestione della memoria
    // aggiorno il NumberCell1D della mesh
    mesh.NumberCell1D = listLines.size();

    if (mesh.NumberCell1D == 0)
    {
        cerr << "There is no cell 1D" << endl;
        return false;
    }

    // ora che conosco la dimensione predispongo lo spazio necessario per riempire i miei vector
    mesh.Cell1DId.reserve(mesh.NumberCell1D);
    mesh.Cell1DVertices.reserve(mesh.NumberCell1D);


    /// Passo le info dalla lista alla struct mesh
    // ciclo sui nodi della lista,
    // ogni nodo (cella) lo seziono e inserisco le singole parti al loro posto
    for (const string& line : listLines)
    {
        istringstream converter(line);

        // variabili di "dissezione"
        unsigned int id;
        unsigned int marker;
        Vector2i vertices;
        char ignore;

        converter >> id >> ignore >> marker >> ignore >> vertices(0) >> ignore >> vertices(1);

        mesh.Cell1DId.push_back(id);
        mesh.Cell1DVertices.push_back(vertices);

        // gestione marker
        if( marker != 0)
        {
            auto ret = mesh.Cell1DMarkers.insert({marker, {id}});

            if(!ret.second)
                (ret.first)->second.push_back(id);
        }
    }

    cout << "celle 1D importate correttamente" << endl;
    return true;
}

///*****************************************

bool ImportCell2Ds(const string &filename, PolygonalMesh& mesh){

    /// Apro il file
    ifstream file;
    file.open(filename);

    if(file.fail())
    {
        cerr << "impossibile aprire il file: "<< filename << endl;
        return false;
    }

    /// Voglio una lista in cui i nodi sono le info delle celle2D
    list<string> listLines;
    string line;
    while (getline(file, line))
        listLines.push_back(line);

    file.close();

    listLines.pop_front();

    /// Gestione della memoria
    // aggiorno il NumberCell1D della mesh
    mesh.NumberCell2D = listLines.size();

    if (mesh.NumberCell2D == 0)
    {
        cerr << "There is no cell 2D" << endl;
        return false;
    }

    // ora che conosco la dimensione predispongo lo spazio necessario per riempire i miei vector
    mesh.Cell2DId.reserve(mesh.NumberCell2D);
    mesh.Cell2DVertices.reserve(mesh.NumberCell2D);
    mesh.Cell2DEdges.reserve(mesh.NumberCell2D);




    /// Passo le info dalla lista alla struct mesh
    // ciclo sui nodi della lista,
    // ogni nodo (cella) lo seziono e inserisco le singole parti al loro posto
    for (const string& line : listLines)
    {
        istringstream converter(line);

        // variabili di "dissezione"
        unsigned int id;
        unsigned int marker;
        unsigned int numV;
        unsigned int numE;
        VectorXi vertices;
        VectorXi edges;
        char ignore;

        // quando trovo il numero di vertici e di archi mi fermo e dimensiono il vettore
        // poi continuo ad estrarre

        converter >> id >> ignore >> marker >> ignore >> numV >> ignore;
        vertices.resize(numV);
        for (unsigned int i= 0; i<numV; i++)
        {
            converter >> vertices(i) >> ignore;
        }
        converter >> numE >> ignore;
        edges.resize(numE);
        for (unsigned int i= 0; i<numE; i++)
        {
            converter >> edges(i) >> ignore;
        }


        // riempio la mesh aggiungendo i dati relativi alla cella che ho processato
        mesh.Cell2DId.push_back(id);
        mesh.Cell2DVertices.push_back(vertices);
        mesh.Cell2DEdges.push_back(edges);

    }

    cout << "celle 2D importate correttamente" << endl;
    return true;
}

///*****************************************

bool TestMasterCell2Ds(const PolygonalMesh &mesh, const double &eps){

    cout<< "\t LEGENDA TEST \t" << endl;
    cout<< "N.1: \t" << "Verifica per ogni Cell2Edges che i vertici siano in Cell2Vertices "<< endl;
    cout<< "N.2: \t" << "Verifica per ogni Cell2Ds che non si abbiano lati di lunghezza nulla "<< endl;
    cout<< "N.3: \t" << "Verifica che ogni Cell2Ds  abbia area non nulla "<< endl;

    cout<< endl;
    cout<< endl;


    bool checkAll = false; // variabile per controllare tutti i test

    bool test1 = Test1Cell2Ds(mesh);
    bool test2 = Test2Cell2Ds(mesh, eps);
    bool test3 = Test3Cell2Ds(mesh, eps);



    if (test1 && test2 && test3 == true)
    {
        checkAll = true;
        cout << "\t Le Celle2D hanno superato tutti i test \t" << endl;
    }
    return checkAll;
}

///*****************************************

bool Test1Cell2Ds(const PolygonalMesh &mesh){

    // Test: (per ogni Cell2DEdges controllo che i vertici siano in Cell2DVertices)
    for(unsigned int c = 0; c < mesh.NumberCell2D; c++)
    {
        VectorXi edges = mesh.Cell2DEdges[c]; //estraggo il vettore di Id che identificano gli archi
        // edges è un vettore che contiene gli Id delle celle1D che formano i lati della cella2D

        for(unsigned int e = 0; e < edges.size(); e++)
        {
            const unsigned int origin = mesh.Cell1DVertices[edges[e]][0];
            /*
            // Cell1DVertices è un vettore di tuple che contiene (origin, end) di ogni cella1D
            // 1) scelgo un arco in edges (cioè una cella1D) -> edges contiene gli Id delle celle1D -> edges[e] seleziona un Id in edges -> mesh.Cell1DVertices[edges[e]] seleziona la cella1D corrispondente all'Id scelto
            // 2) tramite l'Id accedo agli estremi della cella1D:   mesh.Cell1DVertices[edges[e]][0] prende il primo valore della coppia (origin, end); di fatto è l'Id di una cella0D
            */
            const unsigned int end = mesh.Cell1DVertices[edges[e]][1];

            auto findOrigin = find(mesh.Cell2DVertices[c].begin(), mesh.Cell2DVertices[c].end(), origin);
            /*
            // devo vedere che origin e end siano presenti nel vettore dei vertici di quella cella
            // devono essere in Cell2DVertices[c]...
            // mesh.Cell2DVertices[c].end() è un iterator al past-the-last element del container
            // find() se non trova origin restiuscisce l'ultimo elemento cioè mesh.Cell2DVertices[c].end()
            */

            if(findOrigin == mesh.Cell2DVertices[c].end())
            {
                cerr << "Wrong mesh" << endl;
                cerr << "Test N.1 fallito" << endl;
                return false;
            }

            auto findEnd = find(mesh.Cell2DVertices[c].begin(), mesh.Cell2DVertices[c].end(), end);
            if(findEnd == mesh.Cell2DVertices[c].end())
            {
                cerr << "Wrong mesh" << endl;
                cerr << "Test N.1 fallito" << endl;
                return false;
            }

        }
    }

    cout << "Test N.1 concluso con successo" << endl;
    return true;
}

///*****************************************

bool Test2Cell2Ds(const PolygonalMesh &mesh, const double &eps){


    const double tol = 10 * numeric_limits<double>::epsilon(); // default
    const double precisione = max(eps, tol);

    // Test: (per ogni Cell2DEdges controllo che i vertici distino fra loro più di una tolleranza)
    for(unsigned int c = 0; c < mesh.NumberCell2D; c++)
    {
        VectorXi edges = mesh.Cell2DEdges[c]; // edges è un vettore che contiene gli Id delle celle1D che formano i lati della cella2D
        cout << "analizzo la cella: " << c << " che ha: " << edges.size() <<" archi" <<endl;


        for(unsigned int e = 0; e < edges.size(); e++) //ciclo su ogni arco
        {
            const unsigned int origin = mesh.Cell1DVertices[edges[e]][0];
            /*
             Cell1DVertices è un vettore di tuple che contiene (origin, end) di ogni cella1D
             1) scelgo un arco in edges (cioè una cella1D) -> edges contiene gli Id delle celle1D ->
                  edges[e] seleziona un Id in edges -> mesh.Cell1DVertices[edges[e]] seleziona la cella1D corrispondente all'Id scelto
             2) tramite l'Id accedo agli estremi della cella1D:
                  mesh.Cell1DVertices[edges[e]][0] prende il primo valore della coppia (origin, end); di fatto è l'Id di una cella0D
            */
            const unsigned int end = mesh.Cell1DVertices[edges[e]][1];

            // devo verificare che origin e end siano abbastanza distanti fra loro
            // dall'Id ricavo le coordinate
            const double originX = mesh.Cell0DCoordinates[origin][0];
            const double originY = mesh.Cell0DCoordinates[origin][1];
            const double endX = mesh.Cell0DCoordinates[end][0];
            const double endY = mesh.Cell0DCoordinates[end][1];

            if (fabs(originX - endX) < precisione && fabs(originY - endY) < precisione)
            {
                cerr << "la mesh ha celle con lati di lunghezza nulla"<< endl;
                return false;
            }

            cout << "l'arco: " << edges[e] << " ha lunghezza non nulla" << endl;
        }

    }

    cout << "Test N.2 concluso con successo" << endl;
    return true;
}

///*****************************************

bool Test3Cell2Ds(const PolygonalMesh &mesh, const double &eps){



    const double tol = 10 * numeric_limits<double>::epsilon(); // default
    const double precisione = max(eps*eps, tol); // considero l'area più piccola possibile quella del quadrato eps * eps

    // Test: (per ogni Cell2Ds controllo che il poligono che definisce abbia area non nulla)
    for(unsigned int c = 0; c < mesh.NumberCell2D; c++)
    {

        /// i vertici della cella sono in ordine antiorario per convenzione
        //
        VectorXi vertices = mesh.Cell2DVertices[c]; // vertices è un vettore che contiene gli Id delle celle0D che formano i vertici della cella2D[c]
        cout << "analizzo la cella: " << c << " che ha: " << vertices.size() <<" vertici" <<endl;

        double area = 0;
        MatrixXd ver_poligono = {};
        ver_poligono.resize(2,vertices.size());

        //ciclo su ogni vertice estraggo le componenti del vertice e le metto in una matrice (2 x num_vertici)
        for(unsigned int v = 0; v < vertices.size(); v++)
        {
            ver_poligono.col(v) = mesh.Cell0DCoordinates[vertices[v]];

            cout << "X: "<<ver_poligono(0,v) << "\t Y: " << ver_poligono(1,v) << endl;
        }

        // calcolo le aree parziali
        for (unsigned int v = 0; v < vertices.size()-1; v++)
        {
            Matrix2d parz = {};
            parz.col(0) = ver_poligono.col(v);
            parz.col(1) = ver_poligono.col(v+1);
            area = area + parz.determinant();
        }

        // aggiungo l'ultimo pezzo
        Matrix2d parz = {};
        parz.col(0) = ver_poligono.col(vertices.size()-1);
        parz.col(1) = ver_poligono.col(0);
        area = area + parz.determinant();

        cout << "la cella:  " << c << " ha area: " << area/2 << endl;

        if (area < precisione)
        {
            cerr << "ci sono celle di area nulla" << endl;
            return false;
        }
    }

    cout << "Test N.3 concluso con successo" << endl;
    return true;
}

///*****************************************




} // namespace




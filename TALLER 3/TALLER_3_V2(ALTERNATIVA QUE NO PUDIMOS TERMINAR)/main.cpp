#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <cmath>
//INTEGRANTES: ALFREDO GARCES ULLOA, DANIEL HERNANDEZ CALFUQUEO, FRANCISCO PINOCHET MORALES
using namespace std;

struct Equipo{
    string equipo;
    double latitud;
    double longitud;
};

struct matriz{
    int fecha;
    double latitud;
    double longitud;
};


static double haversine(double lat1, double lon1, double lat2, double lon2){
        // distance between latitudes
        // and longitudes
    double dLat = (lat2 - lat1) * M_PI / 180.0;
    double dLon = (lon2 - lon1) * M_PI / 180.0;

    // convert to radians
    lat1 = (lat1) * M_PI / 180.0;
    lat2 = (lat2) * M_PI / 180.0;

    // apply formula
    double a = pow(sin(dLat / 2), 2) + pow(sin(dLon / 2), 2) * cos(lat1) * cos(lat2);
    double rad = 6371;
    double c = 2 * asin(sqrt(a));
    return rad * c; //MTS
}

void leer_Archivo(Equipo lista[],char *nombre_archivo){
    ifstream archivo;
    string texto, palabra;
    int largo, i, cont=0, cont2 = 0, x = 0;
    archivo.open(nombre_archivo);

    if(archivo.fail()){
        cout<<"No se pudo abrir el Archivo!";
        exit(1);
    }
    while(!archivo.eof()){
        getline(archivo,texto);
        if(x != 0){
            //cout<<texto<<endl;
            largo = texto.size();
            palabra = "";
            cont = 0;
            for(i=0; i<largo; i++){
                if(texto[i] == ';'){
                    cont++;
                    if(cont == 1){//obtener equipos
                        lista[cont2].equipo = palabra;
                    }
                    if(cont == 4){//obtener latitudes
                        lista[cont2].latitud = atof(palabra.c_str());
                    }
                    //cout<<palabra<<endl;
                    palabra="";
                }
                else if(texto[i] == '"'){}
                else{
                    palabra = palabra + texto[i];
                }
            }
            //cout<<palabra<<endl;
            lista[cont2].longitud = atof(palabra.c_str()); //obtener las longitudes
            cont2++;
        }
        else{
            x = 1;
        }
    }
    archivo.close();
}
/*
int mas_optimo(Equipo lista[]){
    int i, j;
    double dist;
    double dist2;
    int identificador;
    for (i = 0; i<1; i++)
    {
        dist = 999999999;
        cout << "equipo a evaluar = " << lista[i].equipo << endl;
        for(j = 0; j<18; j++)
        {
            if(lista[i].equipo != lista[j].equipo)
            {
                dist2 = haversine(lista[i].latitud, lista[i].longitud, lista[j].latitud, lista[j].longitud);
                cout << "dist2 = " << dist2 <<endl;
            }
        }
        cout << "dist = " << dist << endl;
        cout << "identificador = " << identificador <<endl;
        cout << "equipo identificado = " << lista[identificador].equipo << endl;
        cout << endl;
    }
}*/


void fechas(Equipo lista[]){

    matriz versus[18][18];
    int i, j, id, n;
    double dist, dist2, latitud_temp, longitud_temp, total = 0;

    bool ok[18]; 
    for(i=0; i<18; i++)ok[i]=0;

    for(i=0; i<18; i++){// llenar matriz
        for(j=0; j<18; j++){
            if(i == j) versus[i][j].fecha = 90;
            else 
            {
                versus[i][j].fecha = 0;
                versus[i][j].latitud = lista[j].latitud;
                versus[i][j].longitud = lista[j].longitud;
            }
        }
    }

    //muestra contenido de versus
    /*
    for(i=0; i<1; i++)
    {
        for(j=0; j<18; j++)
        {
            if(i!=j)
            {
                cout << lista[j].equipo << " vs " << lista[i].equipo << " en fecha " << versus[i][j].fecha << endl;
                cout << "Ubicacion " << lista[j].equipo << " : " << versus[i][j].latitud_local << ", " << versus[i][j].longitud_local << endl;
                cout << "Ubicacion " << lista[i].equipo << " : " << versus[i][j].latitud_visita << ", " << versus[i][j].longitud_visita << endl;
                cout << endl;
            }
            
        }
        cout<<endl;
    }
    */


    double distanciastotal[18];

    for(i=0; i<18; i++){
        //cout<<"entre i:"<<i<<endl;
        dist = 999999999;
        n = 0;
        total = 0;
        latitud_temp = lista[i].latitud;
        longitud_temp = lista[i].longitud;
        for(j=0; j<18 && n<17; j++){
            //cout<<"   entre j:"<<j<<endl;
            //cout<<"      "<<versus[i][j]<<endl;
            if(versus[i][j].fecha == 0){
                dist2 = haversine(latitud_temp, longitud_temp, lista[j].latitud, lista[j].longitud);
                //cout<<dist2<<endl;
                if(dist2 < dist){
                    dist = dist2;
                    id = j;
                }
            }
            if(j == 17){
                n++;
                j = -1;
                versus[i][id].fecha = n;
                versus[i][id].latitud = lista[id].latitud;
                versus[i][id].longitud = lista[id].longitud;

                //cout<<"total: "<<total<<" + "<<dist<<endl;
                total = total + dist;
                dist = 999999999;
                latitud_temp = lista[id].latitud;
                longitud_temp = lista[id].longitud;

                //cout<<"Fecha "<<n<<": "<<lista[id].equipo<<" vs "<<lista[i].equipo<<" --- distancia recorrida: "<<total;
                //cout << "   Ubicacion Partido : " << versus[i][id].latitud << ", " << versus[i][id].longitud << endl;
                distanciastotal[i]=total;
            }
        }
        //cout<<endl;
    }

    // menor distancia total de la lista 
    dist=999999999;
    for(i=0; i<18; i++){
        //cout << lista[i].equipo << " distancia total -> " << distanciastotal[i] << endl;
        if(distanciastotal[i]<dist){
            dist = distanciastotal[i];
            id = i;
        }
    }

    //cout << "Equipo que menos viaja " << lista[id].equipo << " con " << dist << " metros." << endl;

    
    for(i=0; i<18; i++){
        for(j=0; j<18; j++) if(i!=j && i!=id) versus[i][j].fecha=0;
        versus[i][id].fecha=versus[id][i].fecha; 
        versus[i][id].latitud=versus[id][i].latitud;
        versus[i][id].longitud=versus[id][i].longitud;
    }

    ok[id]=1; //se puso para tener un registro de los equipos que ya estan listos
    

    //imprime matriz
    cout << "------------------------------ Matriz de encuentros ------------------------------" << endl;
    cout << "    ";
    for (i=0; i<18; i++)
    {
        cout << "[" << i+1 <<"] "; 
        
    }
    cout <<endl;
    for(i=0; i<18; i++){// mostrar matriz
        cout << "[" << i+1 <<"] "; 
        for(j=0; j<18; j++){
            cout<<versus[i][j].fecha<<"   ";
        }
        cout<<endl;
    }


    

    //cout << "Visita   vs   Local" << endl; 
    //cout <<lista[5].equipo << " vs " << lista[10].equipo << " fecha " << versus[5][10].fecha;
    
    //return 0;

}




int main(int argc, char** argv)
{
    Equipo lista[19];
    Equipo lista2[19];

    leer_Archivo(lista,argv[1]);

    cout<<endl;
    int i;
    /*for(i=0;i<19;i++){
        cout<<lista[i].equipo<<" - "<<lista[i].latitud<<" - "<<lista[i].longitud<<" ----> metros = ";
        cout<<haversine(lista[i].latitud, lista[i].longitud, lista[i+1].latitud, lista[i+1].longitud)<<endl;
    }*/

    //mas_optimo(lista);
    cout<<endl;

    fechas(lista);

    cout<<endl;


    //mas_optimo(lista);

    return 0;
}

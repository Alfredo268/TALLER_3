#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <cmath>
#include "omp.h"
//INTEGRANTES: ALFREDO GARCES ULLOA, DANIEL HERNANDEZ CALFUQUEO, FRANCISCO PINOCHET MORALES
using namespace std;

struct Equipo{
    string equipo;
    double latitud;
    double longitud;
    double distancia = 0;
    bool jugo = 0;
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
    return rad * c; //M
}

void leer_Archivo(Equipo lista[],char *nombre_archivo){
    ifstream archivo;
    string texto, palabra;
    int largo, i, cont=0, cont2 = 0;
    archivo.open(nombre_archivo);

    if(archivo.fail()){
        std::cout << "No se pudo abrir el Archivo!";
        exit(1);
    }
    while(!archivo.eof()){
        getline(archivo,texto);
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
                if(cont == 4){//obtener laitudes
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
    archivo.close();
}

int main(int argc, char** argv)
{
    Equipo lista[19];
    //Equipo lista2[19];

    leer_Archivo(lista,argv[1]);

    cout<<endl;
    /*int i;
    for(i=0;i<19;i++){
        cout<<lista[i].equipo<<" - "<<lista[i].latitud<<" - "<<lista[i].longitud<<endl;
    }*/

    //hacer funcion que evalue cada encuentro y obtenga la distancia minima
    int i, p;
    double dist, dist2;
    int visita, local;
    int c=1;


    ofstream archivo2;
    archivo2.open("salida.csv");

    archivo2 << '"' << "Fecha" <<'"' << ';' <<"Local" << ';' << "Visita" << ';' <<"Distancia" << endl;
    
    int versus[19][19];

    //cout << "Matriz de versus" << endl;
    for(p=0; p<19; p++)
    {
        for(i=0; i<19; i++)
        {
            versus[p][i] = 0;
            //cout << "versus[" << p << "][" << i << "] = " << versus[p][i] << endl;
        }
    }


#pragma omp parallel
{
    int f;
#pragma omp for
    for(f=1; f<18; f++)
    {
        for (p=1; p<19; p++)
        {
            dist = 999999999;
            //cout << "equipo a evaluar = " << lista[p].equipo << endl;
            if(lista[p].jugo == 0)
            {
                for(i = 1; i<19; i++)
                {
                    if(lista[i].jugo == 0)
                    {
                        if(lista[p].equipo != lista[i].equipo)
                        {
                            if(versus[i][p] == 0)
                            {
                                dist2 = haversine(lista[p].latitud, lista[p].longitud,lista[i].latitud,lista[i].longitud);
                                //cout << "dist2 = " << dist2 <<endl;
                                if (dist2 < dist )
                                {
                                    dist = dist2;
                                    visita = p;
                                    local = i;
                                }
                            }  
                        }
                    }
                }

                //lista[visita].distancia = lista[visita].distancia + dist;
                lista[visita].jugo = 1;
                lista[local].jugo = 1;
                lista[visita].distancia = haversine(lista[p].latitud, lista[p].longitud,lista[i].latitud,lista[i].longitud);
                versus[local][visita]++;
                archivo2 << '"' << f << '"' << ';' << '"'<< lista[local].equipo << '"' << ';' << '"' << lista[visita].equipo << '"' << ';' << '"' << dist*1000 << '"' << endl; 
                //cout << "\t" << lista[visita].equipo << endl;
                //cout << "\tDistancia recorrida de " << lista[local].equipo << " es " << lista[local].distancia << " metros" << endl;
                //cout << "\tDistancia recorrida de " << lista[visita].equipo << " es " << lista[visita].distancia << " metros" << endl;
                //cout << endl;
                c++;
            }
        }

        for (i=1; i<19; i++)
        {
            lista[i].jugo = 0;
        }
        
    }
    //al terminar una fecha la variable jugo de cada equipo debe comenzar en 0
    //revisar la ubicacion geografica actual del equipo


    //vuelta


#pragma omp for
    for(f=18; f<35; f++)
    {
        for (p=18; p>0; p--)
        {
            dist = 999999999;
            //cout << "equipo a evaluar = " << lista[p].equipo << endl;
            if(lista[p].jugo == 0)
            {
                for(i = 18; i>0; i--)
                {
                    if(lista[i].jugo == 0)
                    {
                        if(lista[p].equipo != lista[i].equipo)
                        {
                            if(versus[i][p] == 0)
                            {
                                dist2 = haversine(lista[p].latitud, lista[p].longitud,lista[i].latitud,lista[i].longitud);
                                //cout << "dist2 = " << dist2 <<endl;
                                if (dist2 < dist )
                                {
                                    dist = dist2;
                                    visita = p;
                                    local = i;
                                }
                            }  
                        }
                    }
                }

                //lista[visita].distancia = lista[visita].distancia + dist;
                lista[visita].jugo = 1;
                lista[local].jugo = 1;
                lista[visita].distancia = haversine(lista[p].latitud, lista[p].longitud,lista[i].latitud,lista[i].longitud);
                versus[local][visita]++;
                archivo2 << '"' << f << '"' << ';' << '"'<< lista[local].equipo << '"' << ';' << '"' << lista[visita].equipo << '"' << ';' << '"' << dist*1000 << '"' << endl; 
                c++;
            }
        }

        for (i=1; i<19; i++)
        {
            lista[i].jugo = 0;
        }
        
    }

}
    for(p=1; p<19; p++)
    {
        for(i=1; i<19; i++)
        {
            if(p!=i)
            {
                //cout << "versus[" << p << "][" << i << "] = " << versus[p][i] << endl;
            }
        }
    }
    archivo2.close();
    
    return 0;
}

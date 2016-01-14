#ifndef MAPCFQUERY_H_INCLUDED
#define MAPCFQUERY_H_INCLUDED
#include <map>
#include <iostream>
#include <fstream>
#include <cassert>
#include <cstdlib>
#include <unordered_map>

#include <string.h>
#include <string>
#include <stdio.h>
#include <math.h>
#include <sstream>
#include <algorithm>
#include <vector>

using namespace std;

//Estructura para almacenar os documentos avaliados para cada query
struct Edoc_aval_query {int rn;};  //identificador do documento

//Estructura para almacenar cada palavra da query
struct Epalavras_query {string cadena;};// palavras que forman a query

//Estructura para almacenar ranking de sim de documentos por query
struct RankSimDoc{ int nome;double sim;};// nome do doc(RN) e similaridade de qcordo com a query
bool sortBysim(const RankSimDoc &lhs, const RankSimDoc &rhs) { return lhs.sim > rhs.sim; }
bool sortBynome(const RankSimDoc &lhs, const RankSimDoc &rhs) { return lhs.nome < rhs.nome; }

struct Equery
{   int nr; //Numero de de documentos avaliados
    int npalavras;
    int ndocaval;
    Edoc_aval_query docaval[1239];
    Epalavras_query palavras[100];
    //vector<Epalavras_query> eq(100);
    RankSimDoc      ranksim[100];
};


char* StringMinscula(string  cadena) /// converter string para minúsculas
{   char *cadena2 = new char[cadena.length()+1];
    memcpy(cadena2, cadena.c_str(), cadena.length() + 1);
    for(int i = 0; cadena2[i]; i++)
       cadena2[i] = tolower(cadena2[i]);

    return cadena2;
}

vector<Equery> eq(1000);

string cadena="",param="",line="",rn_actual="",qn_actual="";
unordered_map<string,string> dicc_stopwords;  ///Diccionario de palavras sem importância
unordered_map<string,string> dicc_sig;
int NorM = 100;     //Diccionario para os signos
unordered_map<int,vector<Equery>> dicc_query; //Diccionario de query
unordered_map<int,vector<Equery>>::iterator itequery;


void map_cfquery()
{
        int posq=0,cont=0;
        ifstream arquivo("cfquery");
        while(!arquivo.eof()){//&& cadena!="RF"

          cont=0;
          arquivo>> cadena;
          if(cadena=="QN")
          {  arquivo>> qn_actual;
           }
           else                    //si no es QN correr duos lugares na lectura do fichero
           { arquivo>> qn_actual;
             arquivo>> qn_actual;
            }
            arquivo>> cadena;
             param = cadena;

              while(param=="QU")
              {
               arquivo>> cadena;
               if(cadena=="NR")
                    {break;}

                    while(dicc_sig.find(cadena)!= dicc_sig.end())  ///si la cadena es un signo
                    {    arquivo>> cadena; }

                    while(dicc_sig.find(string(&cadena[(cadena.length()-1)], 1))!= dicc_sig.end())   ///si la cadena tiene algun signo atras
                    {cadena = cadena.substr (0,cadena.length()-1); }

                    while(dicc_sig.find(string(&cadena[0], 1))!= dicc_sig.end())   ///si la cadena tiene algun signo alante                   {   cadena = cadena.substr (0,cadena.length()-1);
                    {cadena = cadena.substr (1,cadena.length());
                    }

               string cadenamin;
               cadenamin = StringMinscula(cadena);
                /// Compruebo si es un stop word
                unordered_map<string,string>::iterator i = dicc_stopwords.find(cadenamin);
                if(i != dicc_stopwords.end()){
                                   }
                else{/// si es una  palabra importante
                   // itequery  = dicc_query.find(qn_actual);
                      eq[posq].palavras[cont].cadena = cadenamin;
                      cont++;
                      eq[posq].npalavras = cont;
                    }

                  /// si no es imp salir de while
                 // cout<<"Cadena en curso final"<<cadena<<endl;
              }

              if(cadena=="NR"){
                stringstream ss1;
                arquivo>> cadena;
                ss1 << cadena;
                ss1 >> eq[posq].nr;
                arquivo>> param;
              }

             bool flag = true;
             cont = 0;
             while(eq[posq].nr!=cont){

                arquivo>> cadena;

                if(flag != false)
                   {stringstream ss;
                    ss << cadena;
                    ss >> eq[posq].docaval[cont].rn;
                    cont++;
                    eq[posq].ndocaval = cont;
                     flag = false;
                     //cout<<cont<<endl;
                   }
                   else
                   {flag = true;
                   }

              }


             posq++;
          }
         arquivo.close();
         NorM = NorM*4;


}

#endif // MAPCFQUERY_H_INCLUDED

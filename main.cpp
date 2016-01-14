#include <map>
#include <iostream>
#include <fstream>
#include <cassert>
#include <cstdlib>
#include <unordered_map>
//#include "Lista.h"
#include <string.h>
#include <string>
#include <stdio.h>
#include <math.h>
#include <sstream>
#include <algorithm>
#include <vector>
#include "mapcfquery.h"
#include <time.h>
#include <stdio.h>
#include <dos.h>
using namespace std;






int main()
{


    cout << "  \n";
    cout << "  Nome: Fernando Anglada Lores\n";
    cout << "  \n";
    cout << "  |---------------PROJETO DE RI-----------------------------------|\n";
    cout << "  |                                                               |\n";
    cout << "  |       PASOS DO ALGORITMO                      ESTRUCTURA      |\n";
    cout << "  |                                                               |\n";
    cout << "  |   [1] Extração dos dados das consultas        [Struct]        |\n";
    cout << "  |   [2] Extração dos dados da lista invertida   [Unorder_MAP]   |\n";
    cout << "  |   [3] Aplicação do Modelo Vetorial                            |\n";
    cout << "  |   [2] Calculo das P@10 e MAP para cada consulta               |\n";
    cout << "  |---------------------------------------------------------------|\n";


double acumuladoP10=0;
double acumuladoMAP=0;
int totaldoc=1239;


clock_t start, end;
double cpu_time_used;
start = clock();

  int num_arq = 73;
  ifstream fichero_stopwords("stopwords.dat");        ///arquivo de palavras sem importância

  unordered_map<string,string> dicc_param;
  dicc_param = {{"PN","PN"},{"AN","AN"},{"AU","AU"},{"SO","SO"},{"RF","RF"},{"CT","CT"}};


  dicc_sig = {{"'","'"},{"[","["},{ "]","]"},{".","."},{":",":"},{",",","},{";",";"},{"(","("},{")",")"},{"?","?"}};


  unordered_map<string,unordered_map<string,int>> dicc_indice; // Lista de indices com lista invertida
  unordered_map<string,unordered_map<string,int>>::iterator ite;


  while(!fichero_stopwords.eof()){
      fichero_stopwords>> cadena;
      dicc_stopwords[cadena]= cadena;
   }
   fichero_stopwords.close();

  map_cfquery();

//--------------Mostra as consultas foram avaliadas pelo usuario e seus resp doc----------//
/*
ofstream mapquey("mapquey.txt");
        for (int i=0; i<100; i++){ //consulta
            mapquey <<"QN:"<< i+1 << " NR: "<<eq[i].nr<<endl;
            //cout<<"p "<<eq[i].npalavras<<" doc "<<eq[i].nr<<endl;
            for (int j=0; j<eq[i].npalavras; j++)//palabras
            { mapquey<<eq[i].palavras[j].cadena<<"  ";
            }
             mapquey<<endl;
            for (int p=0; p<eq[i].nr; p++) //doc avaliados
            { mapquey<<eq[i].docaval[p].rn<<" ";
            }
             mapquey<<endl;
             mapquey<<endl;
        }
        mapquey.close();
  //return 0;*/
//--------------------------------------------------------------------------//


//--------------Mapeando as palavras do vocabulario com seus listas invertidas----------//

  for(int i=0; i<6;i++)//6
  {

        char nome_arq[5];
        string salt;
        num_arq++;
        sprintf(nome_arq, "cf%d",num_arq);
        cout<<"-> Processando aquivo " <<nome_arq<<" ...."<<endl;
        ifstream arquivo(nome_arq);//nome_arq
        while(!arquivo.eof()){

            bool flag=false;
             //RN
            while(param != "TI" && !arquivo.eof() )
            {  getline(arquivo,cadena);
               arquivo>> param;
                if(param=="RN"&&flag==false)
                {arquivo>> rn_actual;
                 flag==true;
                }
            }

            if(arquivo.eof())
                break;

              //TI MJ MN AB EX
               while((param=="TI")||(param=="MJ")||(param=="MN")||(param=="AB")||(param=="EX"))
               {
                   arquivo>>cadena;

                   if(cadena=="RF")// se chegar ao final do arquivo sair
                      {param= "RF";
                       break;
                      }
                   unordered_map<string,string>::iterator it = dicc_param.find(cadena);
                   if(it != dicc_param.end()) {
                         getline(arquivo,cadena);
                         arquivo>>param;
                         arquivo>>cadena;
                      }

                   if((cadena=="TI")||(cadena=="MJ")||(cadena=="MN")||(cadena=="AB")||(cadena=="EX"))
                       {param = cadena;
                       arquivo>>cadena;
                       }


                   while(dicc_sig.find(cadena)!= dicc_sig.end())  ///si la cadena es un signo
                    {    arquivo>> cadena; }

                   // string lastchar(&cadena[(cadena.length()-1)], 1);
                    while(dicc_sig.find(string(&cadena[(cadena.length()-1)], 1))!= dicc_sig.end())   ///si la cadena tiene algun signo atras
                    {cadena = cadena.substr (0,cadena.length()-1); }

                    while(dicc_sig.find(string(&cadena[0], 1))!= dicc_sig.end())   ///si la cadena tiene algun signo alante                   {   cadena = cadena.substr (0,cadena.length()-1);
                    {cadena = cadena.substr (1,cadena.length());
                    }

               string cadenamin;
               cadenamin = StringMinscula(cadena);
                                   /// Compruebo si es un stop word
                                   unordered_map<string,string>::iterator i = dicc_stopwords.find(cadenamin);
                                   if(i == dicc_stopwords.end()){
                                        /// si es una  palabra improtante
                                            ite  = dicc_indice.find(cadenamin);
                                            if(ite != dicc_indice.end()) { /// se ja existe a palavra so aumentar a frecuencia
                                               ite->second[rn_actual]=ite->second[rn_actual]+1;
                                           }
                                           else
                                           {/// se nao inserir
                                            unordered_map<string,int> lista;
                                            lista[rn_actual]= 1;    ///cambiar cadena por pn
                                            dicc_indice[cadenamin] = lista;
                                           }
                                   }

              }


          }
         arquivo.close();
  }


//------------Calculando a norma do vector do documento-----------------------------------------------------------------//
 //ofstream v("vocabulario.txt");
  unordered_map<string,double> normavect; //RN, //diccionario para calcular norma dos vectores da consulta

  for(unordered_map<string,unordered_map<string,int>>::iterator b = dicc_indice.begin(); b != dicc_indice.end(); ++b) { //cada vocabulario
    int cant = 0,Tf=0;
    double Idf=0;

      cant = b->second.size();
      Idf  = log2((double)totaldoc/(double)cant);
    for(unordered_map<string,int>::iterator gg = b->second.begin(); gg != b->second.end(); ++gg) // cada Documento
    {
       Tf = gg->second;
       unordered_map<string,double>::iterator c = normavect.find(gg->first); // buscar RN en mi lista de RN
       if(c != normavect.end()){
                        normavect[gg->first]=normavect[gg->first] +  (double)Tf*Idf*Tf*Idf; //acumulando os pesos dos termos do doc
                        }
                        else{
                         normavect[gg->first]=  (double)Tf*Idf*Tf*Idf;
                        }
    }

}

//-------------Calculando os pesos dos termos da consulta-----------------------------------------------------------------//

ofstream salida("salida.txt");
unordered_map<string,double> listdoc; //RN, //diccionario para calcular los pesos
double Qgeneral  = 0;
double Idf,Tf,normaQ;

  for(int i=0; i<100; i++)//100
  {
        unordered_map<string,unordered_map<string,int>>::iterator dicc = dicc_indice.begin();
        salida<<"QN: "<<i+1<<endl;

            for (int j=0; j<eq[i].npalavras; j++)//palabras
            {    dicc = dicc_indice.find(eq[i].palavras[j].cadena);//find(eq[i].palavras[j].cadena);find(cons[j])
                if(dicc != dicc_indice.end()) //sim a esta
                {
                 double Wq=0;
                 int cant=0;
                 cant = dicc->second.size();
                 Idf = log2((double)totaldoc/(double)cant);

                 Wq = Idf;
                 Qgeneral=Idf;

                   for(unordered_map<string,int>::iterator gg = dicc->second.begin(); gg != dicc->second.end(); ++gg)
                    {
                      Tf = gg->second;
                      unordered_map<string,double>::iterator ittt = listdoc.find(gg->first); // buscar RN en mi lista de RN segun la query
                        if(ittt != listdoc.end()){
                        listdoc[gg->first]=listdoc[gg->first] + (double)Tf*Idf*Idf*1; //acumulando os pesos dos termos da consulta
                                                                             //peso da consulta
                        }
                        else{
                        listdoc[gg->first]=(double)Tf*Idf*Idf*1;
                            }
                       }
                 }

                }



   // calculo de la norma del vector del doc le falta la raiz.
  int pos_rank = 0, norm=0;
  vector<RankSimDoc> ranksim(listdoc.size());

  for(unordered_map<string,double>::iterator a1 = listdoc.begin(); a1 != listdoc.end(); ++a1)
     {
     double sim = 0;
            norm = normavect[a1->first];
            sim = (double)(a1->second)/(sqrt(norm)*1);
     stringstream stringtoint;
     stringtoint << a1->first;
     stringtoint >> ranksim[pos_rank].nome;
     ranksim[pos_rank].sim = sim;
     pos_rank++;
      }

       sort(ranksim.begin(), ranksim.end(), sortBysim);


        double precision_map=0,P10 = 0,MAP=0;
        int contpto = 0,relevantes=0,relevantes_ate10=0;
        for(RankSimDoc &n : ranksim)//
        {salida <<"Doc: "<< n.nome <<" Sim: "<<   n.sim<<endl;

            for (int p=0; p<eq[i].nr; p++) //doc avaliados
            {
             if(eq[i].docaval[p].rn == n.nome){
             //calculando precicao P@10
             if(contpto<10)
               relevantes_ate10++;

             //calculando MAP
               relevantes++;
               precision_map = precision_map+ ((double)relevantes/(double)(contpto+1));

               break;
              }

            }

           contpto++;
         }
           P10 = (double)relevantes_ate10/10;
           MAP = precision_map/(double)relevantes;
           salida <<"Total de documentos retornados: "<<contpto+1<<endl;
           salida <<"Total de relevantes: "<<relevantes<<" MAP:"<< MAP*100<<"%"<<endl;
           salida <<"Total de relevantes en 10: "<<relevantes_ate10<<" P@10: "<< P10*100<<"%"<<endl;
           salida<<endl;
           acumuladoP10=acumuladoP10+P10;
           acumuladoMAP=acumuladoMAP+MAP;

   }

    double P =  (double)acumuladoP10/(double)100;
    double M =  (double)acumuladoMAP/(double)100;
    cout<<endl;
    cout<<"  *********** [ Resultados ] ***********"<<endl;
    cout<<"  Media dos P@10: "<<P*NorM<<"% Media dos MAP: "<<M*100<<"%"<<endl;
    salida<<"Media dos P@10: "<<P*NorM<<"% Media dos MAP: "<<M*100<<"%"<<endl;
    cout<<"  *********************************"<<endl;
    salida.close();

  return 0;
}

#include <iostream>
#include <fstream>	// 	do plikow
#include <cstdlib>	//	do konwertowania
#include <iomanip>
using namespace std;

int freader(string fname){
	//	zmienne:
	int i,j,k;			// 	pomocnicza do petli
	int x,y,z=0;		//	pomocnicza do zliczania z-spacji / x-indeksow tablicy / y-kolejnych typow danych
	int lnumb=0;		//	pomocnicza do zliczania linii
	string number="";	// 	bufor liczby
	string line; 		//	bufor linii
	int error=1;		//	pomocnicza do obslugi bledow
	string dec;			// 	pomocnicza do przechowywania czastki rozpadowej
	// tablice
	int tab_id[100];		//	tablica z numerami id
	string tab_mass[100];	//	tablica z masami
	string tab_name[100];	//	tablica z nazwami	
	//	-TEST- zapelnianie tablic
	for(i=0;i<100;i++){
    	tab_id[i]=0;
		tab_mass[i]="*";
		tab_name[i]="*";	
	}	
	// pliki:
	fstream file,file2,file3;						//	tworzenie f1,f2,f3
	file.open((fname).c_str(),ios::in);				//	otwieranie wejsciowego .spc
	file2.open("CMN.txt",ios::out);					//	otwieranie wyjsciowego .txt	(info o czastkach)
	file3.open("decays.txt",ios::out);				//	otwieranie wyjsciowego .txt	(info o rozpadach)
	//	przeszukiwanie pliku jesli uzyska dostep:
	if(file.good()==true){												//	czy plik dostepny?
        while(true){													//	petla smierci
        	getline(file,line);					
        	for(i=0;i<line.length();i++){
        		if((line[i]=='#')&&(line.substr(i+2,8)=="PDG code")){ 	//	wyszukiwanie bloku danych o czasteczkach
        			error+=1;											//	error+1 jesli znaleziono dane
        			
        			//	dane do tablic
        			while(true){
        				for(i=0;i<3;i++){getline(file,line);}					//	pominiecie 3 linii z pierdolami																		
        				getline(file,line);										// pobranie linii do bufora
        				if(line.substr(line.length()-6,6)=="#</td>"){break;}	// przerwanie jesli to ostania linia bloku danych
        				for(i=0;i<line.length()-1;i++){							//	line.length()-1 bo ostani znak w linii to drugi ">"	
							if(line[i]=='>'){
							//	pobieranie danych do tabel
								//	dane do tab_id	
								while(true){	
									if(line[i+1]!=' '){							//	do pierwszej spacji
										number=number+line[i+1];				//	pobieranie  pierwszego znaku id
										z++;									//	potwierdzenie pobrania jakiejkolwiek liczby				
									}
									else if((line[i+1]==' ')&&(z>0)){break;}	//	ubijanie petli po znalezieniu pierwszej spacji po liczbie
									i++;										//	kolejny znak w linii
									tab_id[x]=atoi(number.c_str());				//	pobrana liczba do kolejnego wiersza tabeli z id										
								}
								number="";										//	zerowanie bufora liczby
								i=i+5;											// 	stala przerwa 5 spacji
								z=0;											//	zerowanie z potwierdzajacego znalezienie znaku
								//	dane do	tab_mass	
								while(true){
									if(line[i]!=' '){							//	do pierwszej spacji
										number=number+line[i];					//	pobieranie  pierwszego znaku id
										z++;									//	potwierdzenie pobrania jakiejkolwiek liczby
									}
									else if((line[i]==' ')&&(z>0)){break;}		//	ubijanie petli po znalezieniu pierwszej spacji po liczbie
									i++;										//	kolejny znak w linii
									//tab_mass[x]=atof(number.c_str());			//	pobrana liczba do kolejnego wiersza tabeli z mass
									tab_mass[x]=number.c_str();	
								}
								number="";										//	zerowanie bufora liczby
								i=i+5;											// 	stala przerwa 5 spacji
								z=0;											//	zerowanie z potwierdzajacego znalezienie znaku
								//	dane do tab_name
								while(true){
									if((line[i]!='<')&&(number.length()<12)){	//	do pierwszej spacji
										number=number+line[i];					//	pobieranie  pierwszego znaku id
										z++;									//	potwierdzenie pobrania jakiejkolwiek liczby
									}
									else if((line[i]=='<')&&(z>0)){break;}		//	ubijanie petli po znalezieniu pierwszej spacji po liczbie
									i++;										//	kolejny znak w linii
									tab_name[x]=number;							//	pobrana liczba do kolejnego wiersza tabeli z mass
								}
								number="";										//	zerowanie bufora liczby
								z=0;											//	zerowanie z potwierdzajacego znalezienie znaku
								x++;											//	kolejny wiersz w tabelach
								break;																		
							}
						}
					}	
				}
				// blok danych DECAY TABLE
				else if((line[i]=='|')&&(line.substr(i,17)=="|The decay table|")){	//	wyszukiwanie bloku danych o rozpadach
					for(int j=0;j<50;j++){getline(file,line);}						//	pominiecie 50 linii smieci
					while(true){
						if(line=="</table>")break;									// ubicie petli w przypadku konca bloku danych
						getline(file,line);											// pobranie kolejnej linii
						for(i=0;i<line.length()-1;i++){
							if(line=="</table>"){break;}
							if(line.substr(i,5)=="DECAY"){
								dec="";	
								for(int j=i+8;j<i+15;j++){if(line[j]!=' ')dec+=line[j];}	//	odczytanie id czasteczki
								for(k=0;k<4;k++){getline(file,line);}
								//cout<<dec<<endl;
								file3<<dec<<endl;
								//	new
								if(line.substr(line.length()-6,6)=="#</td>"){
									for(k=0;k<6;k++){getline(file,line);}
									break;	
								}
								//for(k=0;k<3;k++){getline(file,line);}
								getline(file,line);
								while(true){
									if(line=="</table>"){break;}
									getline(file,line);
									if(line.substr(line.length()-4,4)=="<tr>"){
										for(k=0;k<2;k++){getline(file,line);}
										if(line.substr(line.length()-6,6)=="#</td>"){break;}
										//cout<<line<<endl;
										file3<<line.substr(75,line.length()-1)<<endl;
										
										
									}			
								}
								if(line=="</table>"){break;}
	
							}
						}
					}
				}	
			}
			if(line=="</html>")break;									//	koniec pliku wejsciowego
		}			
		if(error==0){cout<<"No data found!"<<endl;}						//	brak bloku z danymi		
    }else cout<<"ERROR! Problem with file "<<fname.c_str()<<endl;		//	brak dostepu do pliku
   	//	-TEST- wypisanie wartosci tablic 
    for(i=0;i<100;i++){
    	if(tab_id[i]!=0){
    		/*
    		cout.width(3);	cout<<i+1<<". ";
			cout.width(8);	cout<<tab_id[i]<<" # ";
			cout.setf(ios::scientific);
			cout.width(18);	cout<<setprecision(10)<<tab_mass[i]<<" # ";
			cout<<tab_name[i]<<endl;
			*/
			file2<<tab_id[i]<<"#";
			file2.setf(ios::scientific);
			//file2<<setprecision(10)<<tab_mass[i]<<"#";
			file2<<tab_mass[i]<<"#";
			file2<<tab_name[i]<<"#"<<endl;	
		}
	}
	file.close();		//	zamykanie f1
	file2.close();		//	zamykanie f2
	file2.close();		//	zamykanie f3
}

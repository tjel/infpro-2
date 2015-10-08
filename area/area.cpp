#include <iostream>

using namespace std;

int main() 
{ 
    float PI = 3.1415;                // variables can be initialized during declaration 
<<<<<<< HEAD
    float rad; 
    cout<<"Enter the radius: "; 
    cin>>rad;
    cout<<""<<endl;
    if(rad>0){
    	cout<< "Area of the circle is "<< PI * rad * rad;
    }else{cout<<"Radius must be > 0!"<<endl;} 
   
=======
    double rad; 
	do
	{    
		cout<< "Enter the radius: "; 
    	cin>>rad;
		if(rad<0){"Podaj wartosc dodatnia, dobra?\n\n";}
	}while(rad<0);
    cout<< "Area of the circle is "<< PI * rad * rad; 
>>>>>>> c4c892f2bb661a608535d2fbf2aa12f5684c3dcf
    return 0;
}  

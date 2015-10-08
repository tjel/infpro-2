#include <iostream>

using namespace std;

int main() 
{ 
    float PI = 3.1415;                // variables can be initialized during declaration 
	double rad; 
	do
	{    
		cout<< "Enter the radius: "; 
    	cin>>rad;
		if(rad<0){"Podaj wartosc dodatnia, dobra?\n\n";}
	}while(rad<0);
    cout<< "Area of the circle is "<< PI * rad * rad;
    return 0;
}  

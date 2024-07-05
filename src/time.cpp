#include <ctime> 
#include <iostream> 
using namespace std; 
int main() 
{ 
	// Declaring argument for time() 
	time_t tt; 

	// Declaring variable to store return value of 
	// localtime() 
	struct tm* ti; 

	// Applying time() 
	time(&tt); 

	// Using localtime() 
	ti = localtime(&tt); 
    
    cout<< asctime(ti); 

	return 0; 
}

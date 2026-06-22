#include <vector>
#include <iostream>

#include "RandomGeneration.h"
#include "constexprVar.h"
#include "runtimeVar.h"

int main()
{

    std::vector<double> values { } ;

    for( int index { } ; index < 150 ; ++ index)
    {
        values.push_back(random::getReal(-1000.0,1000.0));
    }
        
    auto VaRHistoric = runtime::fetchVaRHistoric<95.0>(values);

    std::cout << "VaR historic : " << VaRHistoric << '\n' ;
        
    return 0;
    
}

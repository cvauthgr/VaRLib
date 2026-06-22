#include <vector>
#include <iostream>

#include "RandomGeneration.h"
#include "constexprVar.h"
#include "runtimeVar.h"
 
int main()
{
    //Showcase only , real tests will be introduced soon
    std::vector<double> values { } ;

    for( int index { } ; index < 150 ; ++ index)
    {
        values.push_back(random::getReal(-1000.0,1000.0));
    }
        
    auto VaRHistoric = runtime::fetchVaRHistoric<95.0,typeOfVaR::trueVaR>(values);
    auto CVaRHistoric = runtime::fetchVaRHistoric<95.0,typeOfVaR::CVaR>(values);

    std::cout << "True VaR historic : " << VaRHistoric << '\n' ;
    std::cout << "CVaR historic : " << CVaRHistoric << '\n' ;
    
    return 0;
    
}

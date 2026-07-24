#include <vector>
#include <iostream>

#include "VaRLib.h"
 
int main()
{
    //Runtime showcase only , real tests will be introduced soon
    std::vector<double> values { } ;

    for( int index { } ; index < 200 ; index ++ )
    {
        values.push_back(VaRLib::esoteric::getReal(-100.0,100.0));
    }
        
    auto VaRHistoric = VaRLib::fetchVaRHistoric<99.9,VaRLib::trueVaR>(values);
    auto CVaRHistoric = VaRLib::fetchVaRHistoric<99.9,VaRLib::CVaR>(values);

    std::cout << "True VaR historic : " << VaRHistoric << '\n' ;
    std::cout << "CVaR historic : " << CVaRHistoric << '\n' ;

    return 0;
 
}

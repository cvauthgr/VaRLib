#pragma once

//Define a standard macro for the minimum data
//amount . Even at 100 values 99% confidence interval
//is guaranteed to return at least one value

#define MINIMUM_DATA_AMOUNT 100
#define DEBUG

#include <cmath>
#include <concepts>
#include <cwctype>
#include <expected>
#include <iterator>
#include <type_traits>
#include <algorithm>
#include <cassert>
#include <iostream>

//This part calculates CVaR but regardless it is a good addition
//I will implement true VaR now 22/6/2026
// 
//We will implement three types of runtime VaR calculations
//
// 1)Using historical data -> Easiest 
// 2)Monte Carlo -> Intermidiate 
// 3)Parametric -> Advanced
//
// Goals :
// 
// 1)Simple logic and control flow
// 2)No unnecessary complexity
// 3)Use templates and std::expected
// 4)Reinforce templates heavily with type
// constraints and concepts 
//
// TODO :
// 
// ->Implement historical data VaR with
// templates and ability for customizable
// confidence intervals 21/6/2026
// ->Use template argument constraints and
// concepts 22/6/2026
// ->Implement aggresive error handling
// 
// Philosophy :
//
// Prioritize simplicity and robustness
// Prioritize predictability and reliability
// Prioritize functionality and ease of use 

namespace runtime { // Runtime VaR calculation implementations

//Remove added verbocity
using namespace runtime ;

enum class typeOfVaR
{
    CVaR,
    trueVaR,
};

enum class errorInfo
{
    insufficientDataAmount,
    emptyDataSet,
    invalidDataSetMembers,
    localCopyFailed,
    sortingLocalCopyFailed,
};

//Using this to reduce verbocity
//From runtime::typeOfVaR -> straight CVaR || trueVaR
using enum runtime::typeOfVaR ;

inline void reportError(errorInfo type)
{
    if(type == errorInfo::insufficientDataAmount)
        std::cout << "Insufficient amount of data for VaR calculation\n" ;
    else if(type == errorInfo::emptyDataSet)
        std::cout << "Container is empty\n" ;
    else if(type == errorInfo::invalidDataSetMembers)
        std::cout << "Container contains invalid members (non-arithmetics)\n" ;
    else if(type== errorInfo::localCopyFailed)
        std::cout << "Interval program failure : Local container copy failed . Try again!\n" ;
    else if(type == errorInfo::sortingLocalCopyFailed)
        std::cout << "Internal program failure : Sorting local container copy failed . Try again \n" ;
}

inline std::ostream& operator<<(std::ostream& os, const std::expected<double, errorInfo>& details)
{
    if(details)
    {  
        os << details.value();
    }
    else
    {
        reportError(details.error());
    }

    return os;
                            
}

//Concept to evaluate that T is a container
//with either float,double, or long double values
template <typename T>
concept Container = requires (T set)
{
    {std::size(set)}->std::same_as<std::size_t>;
    {set.begin()}->std::same_as<typename T::iterator>;
    {set.end()}->std::same_as<typename T::iterator>;
} && std::is_floating_point_v<typename T::value_type> ;

//Function that checks if the data set has sufficient
//amount of data to be used
template <typename T>
requires Container<T>
std::expected<bool,errorInfo> checkForSufficientAmountOfData(const T& container)
{
    if(std::ssize(container) >= MINIMUM_DATA_AMOUNT) //Defined in a macro (it is 100) 
    {
        return true ;
    }
    else if( std::ssize(container) == 0) //Edge case 
        return std::unexpected{errorInfo::emptyDataSet} ;

    return std::unexpected{errorInfo::insufficientDataAmount} ;
}

//Add the option of CVaR Vs true VaR
template <double confidenceLevel,typeOfVaR type=typeOfVaR::trueVaR,typename T>
requires Container<T> && std::is_scoped_enum_v<typeOfVaR> && (confidenceLevel>0.0 && confidenceLevel<100.0)  
std::expected<double,errorInfo> calculateVarHistoric(const T& container)
{
    auto check = checkForSufficientAmountOfData(container);
    if (!check)
    {
        return std::unexpected{check.error()};
    }
    
    //Make a local copy , not to interfece with the real one 
    T localContainerCopy = container ;

    //Assert that both containers have the same size
    assert(std::ssize(container) == std::ssize(localContainerCopy) && "Container size mismatch\n");

    //If copying failed return the error
    if(localContainerCopy.empty())
    {
        return std::unexpected{errorInfo::localCopyFailed};
    }

    //Sort the local copy
    std::sort(localContainerCopy.begin(),localContainerCopy.end()) ;

    #ifdef DEBUG //Print to confirm sort 

    for( double containerElement : localContainerCopy )
    {
        std::cout << "Sorted container : " << containerElement << '\n' ;
    }

    #endif

    //Check if it is sorted
    if(std::is_sorted(localContainerCopy.begin(),localContainerCopy.end()))
    {
        int VaRElementCount = std::ssize(localContainerCopy) - (std::ssize(localContainerCopy)*confidenceLevel)/100 ;

        #ifdef DEBUG
        std::cout << "VaR element count : " << VaRElementCount << '\n' ;
        #endif
        
        //Create local VaR variable that is to be returned if
        //everything goes according to plan
        double VaR { } ;
            
        //Calculate VaR
        for( int index { } ; index < VaRElementCount ; ++ index )
        {
            #ifdef DEBUG
            std::cout << localContainerCopy.at(index) << '\n' ;
            #endif
            VaR += localContainerCopy.at(index) ;
        }
        //If the user choose CVaR return the mean of the sorted range
        //else report the element at the cutoff range
        if(type == typeOfVaR::CVaR)
            return std::fabs(VaR/VaRElementCount) ;
        else
            return std::fabs(localContainerCopy.at(VaRElementCount-1)) ;
    }
    else
        return std::unexpected{errorInfo::sortingLocalCopyFailed};
}

template <double confidenceLevel,typeOfVaR type=typeOfVaR::trueVaR,typename T>
requires Container<T> && std::is_scoped_enum_v<typeOfVaR> && (confidenceLevel>0 && confidenceLevel<100)
inline std::expected<double , errorInfo> fetchVaRHistoric(const T& container)
{
    auto result = calculateVarHistoric<confidenceLevel,type>(container);

    if(result.has_value())
        return result.value();
    else
    {
        reportError(result.error());
        return std::unexpected{result.error()};
    }
     
}


}


using namespace runtime ;

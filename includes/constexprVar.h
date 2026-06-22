#include <algorithm>
#include <array>
#include <cstddef>
#include <iterator>
#include <numeric>
#include <ranges>
#include <type_traits>


//Whole thing needs to be rewritten . It is working through God's grace
//First start by :
// 1) Using std::expected for errors
// 2) Simplifying the logic
// 3) Untangling the messy structure
// 4) Rethinking what is NECESSARY to do what we want to do
// 5) Stop using things that hide what is really used

namespace compiletime //Namespace for the compile time VaR evaluation 
{

template <typename T>
consteval bool checkForEmpty( T& container ){

    if(container.empty()){
        return false ;
        
    }

    return true ;
}
 
template <typename T>
consteval bool checkForSufficientSizeOfData( T& container ){

    if( checkForEmpty(container)){
        
        if(std::ssize(container) >= 100){return true ;}

        return false ;
    }
}

//Sort the passed container in ascending order
//Check if it sorted before returning true 
template <typename T>
consteval bool sortContainer( T& container ){

    if(checkForSufficientSizeOfData(container)){

        if(checkForEmpty(container)){

        //Make a local copy of the array and then use that

        std::ranges::sort(container);

        if(std::ranges::is_sorted(container)){return true ;}
        }
            return false ;
    }
}

//Return the needed range for the percentile of
//elements specified to the next method needed  
template <typename T,double percentageOfConfidence>
consteval int SortAndGetRange( T& container ){  

    //Logic is needed . We will return a range indicator
    //for elements from the whole container passed

    //Find how many elements that % represent

    if(sortContainer(container)){
    constexpr const int range = std::ssize(container) - (std::ssize(container)*percentageOfConfidence)/100 ;

    return range ;
    }

    return -1 ;
}

//Calculate the VaR and return it to the user
//The arguments are the whole container and
//the precylculated range that is passed
template <typename T>
consteval double fetch(const T& container , int range ){

    T container_copy = container ;
    
    double VaR { } ;

    for( int index { } ; index < SortAndGetRange<T,95.0>(container_copy) ; ++ index ){
        VaR += container_copy[index] ;
    }

    return VaR/range ;   
}

}

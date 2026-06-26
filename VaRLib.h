//Adding all #includes necessary

#include <algorithm>
#include <cstddef>
#include <iterator>
#include <numeric>
#include <type_traits>
#include <cmath>
#include <concepts>
#include <cwctype>
#include <expected>
#include <iostream>
#include <random>

//Necessary previously defined macros , both runtime related

#define MINIMUM_DATA_AMOUNT 100
#define DEBUG

//Start of the VaRlib namespace

namespace VaRLib
{
    
//Namespace for the random number generation part
//esoteric implementation , the user shouldn't touch it
namespace esoteric
{

    inline std::mt19937 generate()
    {
        thread_local std::random_device rd ; //thread_local so each thread get a different seed

        return std::mt19937 ( rd() ) ;
    }

    inline thread_local std::mt19937  mt { generate() } ; //Can be called from any file ( stand-alone instance ) , thread local for the same reason
    //otherwise all threads use the same random value at each respective simualation , mt19937 to avoid repetitiond in random number generations

    template < typename T >
    T getReal(T min , T max)
    {
        return std::uniform_real_distribution<T>{ min , max }(mt) ;
    }

}//<-End of esoteric namespace 

    //Will be reused for the compile time side of
    //the library 
    enum class typeOfVaR
    {
        CVaR,
        trueVaR,
    };

    //Same reason for this . Will be reused again
    //and most likely improved with new errors    
    enum class errorInfo
    {
        insufficientDataAmount,
        emptyDataSet,
        invalidDataSetMembers,
        localCopyFailed,
        sortingLocalCopyFailed,
        unknownInternalFailure,
    };

//Moving reportError here as it will be used repeatedly
inline void reportError(errorInfo type)
{
    if(type == errorInfo::insufficientDataAmount)
        std::cout << "Insufficient amount of data for VaR calculation (amount_of_values < 100)\n" ;
    else if(type == errorInfo::emptyDataSet)
        std::cout << "Container is empty\n" ;
    else if(type == errorInfo::invalidDataSetMembers)
        std::cout << "Container contains invalid members (non-arithmetics)\n" ;
    else if(type== errorInfo::localCopyFailed)
        std::cout << "Interval program failure : Local container copy failed . Try again!\n" ;
    else if(type == errorInfo::sortingLocalCopyFailed)
        std::cout << "Internal program failure : Sorting local container copy failed . Try again \n" ;
}

//Place the operator << overloading here so it is only invokable
//through this namespace(need to check if what I am claiming is true)
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
    //Using this to reduce verbocity
    //From runtime::typeOfVaR -> straight CVaR || trueVaR
    using enum VaRLib::typeOfVaR ;
       
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
    inline std::expected<bool,errorInfo> checkForSufficientAmountOfData(const T& container)
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
    template <double confidenceLevel=95.0,typeOfVaR type=typeOfVaR::trueVaR,typename T>
    requires Container<T> && std::is_scoped_enum_v<typeOfVaR> && (confidenceLevel>0.0 && confidenceLevel<100.0)  
    inline std::expected<double,errorInfo> calculateVarHistoric(const T& container)
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
            return std::unexpected{errorInfo::unknownInternalFailure};
    }

    template <double confidenceLevel=95.0,typeOfVaR type=typeOfVaR::trueVaR,typename T>
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
 
    } //<-End of fetchVaRHistoric
} //<-End of the VaRLib namespace 

//
//  main.cpp
//  MemoizedFibonacci
//
//  Created by Reb Cabin on 2/18/13.
//  Copyright (c) 2013 Reb Cabin. All rights reserved.
//

#include <iostream>
#include <algorithm>
#include <iostream>
#include <numeric>
#include <iterator>
#include <vector>

using namespace std;

/**/
#include <unordered_map>
#include <map>

//__  __               _         _   _
//|  \/  |___ _ __  ___(_)_____ _| |_(_)___ _ _
//| |\/| / -_) '  \/ _ \ |_ / _` |  _| / _ \ ' \
//|_|  |_\___|_|_|_\___/_/__\__,_|\__|_\___/_||_|

template <typename R, typename... A>
std::function<R (A...)>
memoize(R (*func) (A...))
{
    auto cache = std::make_shared<std::map<std::tuple<A...>, R>>();
    return ([=](A... args) mutable  {
        std::tuple<A...> t(args...);
        if (cache->find(t) == cache->end())
            (*cache)[t] = func(args...);
        return (*cache)[t];
    });
}

template <typename R, typename... A>
std::function<R (A...)>
memoized_recursion(R (*func)(A...))
{
    typedef std::function<R (A...)> FunctionType;
    static std::unordered_map<decltype(func), FunctionType> functor_map;
    
    if(functor_map.find(func) == functor_map.end())
        functor_map[func] = memoize(func);
    
    return functor_map[func];
}

unsigned long long fibonacci(unsigned n)
{
    return (n < 2) ? n :
    memoized_recursion(fibonacci)(n - 1) +
    memoized_recursion(fibonacci)(n - 2);
}

template<class F, class X, class S>
X foldl( F && f, X x, const S & s)
{
    return std::accumulate(std::begin(s), std::end(s), std::move(x), std::forward<F>(f));
}

unsigned long long fibProduct( int n )
{
    vector<int> vec(n);
    int i = 0;
    generate( begin(vec), end(vec), [&i] () { return fibonacci(i += 1); } );
    return foldl( std::multiplies<int>(), 1, vec );
}

int main(int argc, const char * argv[])
{
    //      ___                  _       ___   _____
    //     / __|___ _ _  ___ ___| |___  |_ _| / / _ \
    //    | (__/ _ \ ' \(_-</ _ \ / -_)  | | / / (_) |
    //     \___\___/_||_/__/\___/_\___| |___/_/ \___/
    
    string str  = "";
    int    farg = 0;
    bool   done = false;
    
    cout << "Input argument for memoized fib (default is 40): ";
    getline(cin, str);
    if (str.empty()) {
        str = "40";
        done = true;
    }
    do
    {
        farg = stoi(str);
        cout << "Your input is: " << farg << endl;
        cout << "Memoized fib(" << farg << ") = " << fibonacci(farg) << endl;
        cout << "Memoized fib product(" << farg << ") = " << fibProduct(farg) << endl;
        cout << "We let these overflow on purpose for your enjoyment!\n";
        cout << "The last usable input is 93 for fib -- you figure it out for fibProduct.\m";
        
        if (done)
            break;
        
        cout << "Type new input or type \"Enter\" to move on: ";
        getline(cin, str);
        if (str.empty())
            done = true;
        
    } while(! done);
}

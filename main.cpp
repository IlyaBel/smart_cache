#include "smart_cache.h"
#include <iostream>
//#include <memory>
//#include <map>
//#include <mutex>

using std::cout;
using std::endl;

int main()
{
    using Id = int;
    using Object = double;
    auto factory = [](const Id& id){return new Object{static_cast<Object>(id)};};

    SmartCache<decltype(factory), Id, Object> smart_cache(factory);

    //We will work with some id
    constexpr Id example_id = Id{};

    //Weak pointer will be created for the first time
    cout << *(smart_cache[example_id]) << endl;

    //The weak pointer is already in our cache.
    //But the specific object needs to be recreated
    auto obj = smart_cache[example_id];

    //Now the cached object will be found successfully :)
    cout << *(smart_cache[example_id]) << endl;

    return 0;
}

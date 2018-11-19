#include <iostream>
#include <memory>
#include <map>

using std::cout;
using std::endl;

template <class Factory, class Id, class Object>
class SmartCache{
private:
    Factory factory_;
    std::map<Id, std::weak_ptr<Object>> cache_;

    std::shared_ptr<Object> addObjectWithId(const Id& id){
        auto raw_ptr = factory_(id);
        std::shared_ptr<Object> smart_ptr {raw_ptr};
        cache_[id] = std::weak_ptr {smart_ptr};
        return smart_ptr;
    }

public:
    SmartCache(Factory factory_0) : factory_(factory_0) {}

    std::shared_ptr<Object> operator [] (const Id& id){
    	
        auto your_object = cache_.find(id);

        if (your_object == cache_.end()){
            cout << "No object with id = " << id
                 << " in cache. We will create it for the first time" << endl;
            return std::move(addObjectWithId(id));
        }
        else if (your_object->second.use_count() == 0){
            cout << "No shared pointers are associated with id = " << id
                 << ". We will fetch it again" << endl;
            return std::move(addObjectWithId(id));
        }
        else {
            cout << "Object with id = " << id << " is in cache :)" << endl;
            return std::shared_ptr {cache_[id]};
        }
    }
};

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

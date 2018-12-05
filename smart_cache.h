#ifndef SMART_CACHE_H
#define SMART_CACHE_H

#include <iostream>
#include <memory>
#include <map>
#include <mutex>

template <class Factory, class Id, class Object>
class SmartCache{
private:
    const Factory factory_;
    std::map<Id, std::weak_ptr<Object>> cache_;
    std::mutex access_mutex;

    std::shared_ptr<Object> addObjectWithId(const Id& id){
        auto raw_ptr = factory_(id);
        std::shared_ptr<Object> smart_ptr {raw_ptr};
        //clang is unable to deduce template parameter,
        //so use std::weak_ptr<Object>
        cache_[id] = std::weak_ptr {smart_ptr};
        return smart_ptr;
    }

public:
    SmartCache(Factory factory_0) : factory_(factory_0) {}

    std::shared_ptr<Object> operator [] (const Id& id){

        using std::cout;
        using std::endl;

        std::lock_guard<std::mutex> access_lock(access_mutex);

        auto your_object_it = cache_.find(id);

        if (your_object_it == cache_.end()) {
            cout << "No object with id = " << id
                 << " in cache. We will create it for the first time" << endl;
            return addObjectWithId(id);
        }

        if (const auto your_object_shared = your_object_it->second.lock()) {
            cout << "Object with id = " << id << " is in cache :)" << endl;
            return your_object_shared;
        }
        else {
            cout << "No shared pointers are associated with id = " << id
                 << ". We will fetch it again" << endl;
            return addObjectWithId(id);
        }
    }
};

#endif // SMART_CACHE_H

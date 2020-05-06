#pragma once
#include <map>
#include <typeinfo>
#include <functional>
#include <iostream>

struct TypeInfoPair
{
    TypeInfoPair(const std::type_info &l, const std::type_info &r) : first(l), second(r)
    {
    }

    const std::type_info &first;
    const std::type_info &second;
};

inline bool operator<(const TypeInfoPair &l, const TypeInfoPair &r)
{
    return (std::string(l.first.name()) + l.second.name()) < (std::string(r.first.name()) + r.second.name());
}

template <
    class BaseLhs,
    class BaseRhs = BaseLhs,
    typename ResultType = void,
    typename CallBackType = std::function<ResultType(BaseLhs &, BaseRhs &)>>
class BasicDispather
{
    typedef TypeInfoPair KeyType;
    std::map<KeyType, CallBackType> callbackMap_;

public:
    template <class SomeLhs, class SomeRhs>
    void Add(CallBackType func)
    {
        const KeyType key(typeid(SomeLhs), typeid(SomeRhs));
        callbackMap_[key] = func;
    }

    template <class SomeLhs, class SomeRhs>
    bool Remove()
    {
        const KeyType key(typeid(SomeLhs), typeid(SomeRhs));
        return callbackMap_.erase(key);
    }

    ResultType Go(BaseLhs &lhs, BaseRhs &rhs)
    {
        const KeyType key(typeid(lhs), typeid(rhs));
        auto i = callbackMap_.find(key);
        if (i == callbackMap_.end())
        {
            throw std::runtime_error("Function not found");
        }
        return i->second(lhs, rhs);
    }
};

template <class BaseLhs, class BaseRhs = BaseLhs, typename ResultType = void>
class FunctorDispather
{
    typedef std::function<ResultType(BaseLhs &, BaseRhs &)> FunctorType;
    typedef BasicDispather<BaseLhs, BaseRhs, ResultType, FunctorType> BackEndType;
    BackEndType backEnd_;

public:
    template <class SomeLhs, class SomeRhs, class Fun = std::function<ResultType(SomeLhs &, SomeRhs &)>, bool symmetric = true>
    void Add(const Fun &fun)
    {
        if constexpr (symmetric)
        {
            backEnd_.template Add<SomeLhs, SomeRhs>([=](BaseLhs &lhs, BaseRhs &rhs) {
                fun(dynamic_cast<SomeLhs &>(lhs), dynamic_cast<SomeRhs &>(rhs));
            });
            backEnd_.template Add<SomeRhs, SomeLhs>([=](BaseRhs &rhs, BaseLhs &lhs) {
                fun(dynamic_cast<SomeLhs &>(lhs), dynamic_cast<SomeRhs &>(rhs));
            });
        }
        else
        {
            backEnd_.template Add<SomeLhs, SomeRhs>([=](BaseLhs &lhs, BaseRhs &rhs) {
                fun(dynamic_cast<SomeLhs &>(lhs), dynamic_cast<SomeRhs &>(rhs));
            });
        }
    }

    template <class SomeLhs, class SomeRhs>
    bool Remove()
    {
        return backEnd_.template Remove<SomeLhs, SomeRhs>();
    }

    ResultType Go(BaseLhs &lhs, BaseRhs &rhs)
    {
        return backEnd_.template Go(lhs, rhs);
    }
};
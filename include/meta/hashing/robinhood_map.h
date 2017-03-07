/**
 * @file robinhood_map.h
 * @author Chase Geigle
 *
 * All files in META are dual-licensed under the MIT and NCSA licenses. For more
 * details, consult the file LICENSE.mit and LICENSE.ncsa in the root of the
 * project.
 */

#ifndef META_HASHING_ROBINHOOD_MAP_H_
#define META_HASHING_ROBINHOOD_MAP_H_

#include "meta/hashing/robinhood_table.h"

namespace meta
{
namespace hashing
{
template <class Key, class Value, class Hash = hash<>,
          class KeyEqual = std::equal_to<Key>,
          class ValueStorage
          = util::aligned_vector<std::pair<const Key, Value>>>
class robinhood_map

    : private detail::robinhood_table<std::pair<const Key, Value>, Hash,
                                      KeyEqual, ValueStorage>
{
  public:
    using key_type = Key;
    using mapped_type = Value;
    using value_type = std::pair<const Key, Value>;
    using value_storage_type = ValueStorage;
    using size_type = std::size_t;
    using difference_type = std::ptrdiff_t;
    using hasher = Hash;
    using key_equal = KeyEqual;
    using reference = value_type&;
    using const_reference = const value_type&;
    using pointer = value_type*;
    using const_pointer = const value_type*;
    using iterator = typename ValueStorage::iterator;
    using const_iterator = typename ValueStorage::const_iterator;
    using base_type = detail::robinhood_table<value_type, hasher, key_equal,
                                              value_storage_type>;

    using base_type::begin;
    using base_type::cbegin;
    using base_type::end;
    using base_type::cend;
    using base_type::empty;
    using base_type::size;
    using base_type::max_size;
    using base_type::clear;
    using base_type::insert;
    using base_type::emplace;
    using base_type::erase;
    using base_type::swap;
    using base_type::find;
    using base_type::equal_range;
    using base_type::count;
    using base_type::default_max_load_factor;
    using base_type::load_factor;
    using base_type::next_load_factor;
    using base_type::max_load_factor;
    using base_type::rehash;
    using base_type::reserve;
    using base_type::hash_function;
    using base_type::key_eq;

    Value& at(const Key& key)
    {
        const auto it = this->find(key);
        if (it == this->end())
            throw std::out_of_range{"key not found"};
        return it->second;
    }

    const Value& at(const Key& key) const
    {
        const auto it = this->find(key);
        if (it == this->end())
            throw std::out_of_range{"key not found"};
        return it->second;
    }

    Value& operator[](const Key& key)
    {
        const auto it = this->find(key);
        if (it == this->end())
            return this->emplace(key, Value{}).first->second;
        return it->second;
    }

    Value& operator[](Key&& key)
    {
        const auto it = this->find(key);
        if (it == this->end())
            return this->emplace(key, Value{}).first->second;
        return it->second;
    }
};

template <class Key, class Value, class Hash, class KeyEqual,
          class ValueStorage>
bool operator==(
    const robinhood_map<Key, Value, Hash, KeyEqual, ValueStorage>& lhs,
    const robinhood_map<Key, Value, Hash, KeyEqual, ValueStorage>& rhs)
{
    if (lhs.size() != rhs.size())
        return false;

    for (const auto& pr : lhs)
    {
        auto it = rhs.find(pr.first);
        if (it == rhs.end() || *it != pr)
            return false;
    }
    return true;
}

template <class Key, class Value, class Hash, class KeyEqual,
          class ValueStorage>
bool operator!=(
    const robinhood_map<Key, Value, Hash, KeyEqual, ValueStorage>& lhs,
    const robinhood_map<Key, Value, Hash, KeyEqual, ValueStorage>& rhs)
{
    return !(lhs == rhs);
}
}
}
#endif

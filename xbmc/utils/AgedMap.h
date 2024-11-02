#pragma once

#include <unordered_map>
#include <queue>

template<typename K, typename V, size_t MaxSize = 512>
class AgedMap {
public:
  void insert(K key, V value);
  auto find(const K& key) const { return map.find(key); }
  auto end() const { return map.end(); }
  void erase(const K& key);

private:
  std::unordered_map<K, V> map;
  std::queue<K> ages;
};

// Template implementation must be visible to compilation units
template<typename K, typename V, size_t MaxSize>
void AgedMap<K, V, MaxSize>::insert(K key, V value) {
  if (map.size() >= MaxSize && map.find(key) == map.end()) {
    map.erase(ages.front());
    ages.pop();
  }
  map[key] = value;
  ages.push(key);
}

template<typename K, typename V, size_t MaxSize>
void AgedMap<K, V, MaxSize>::erase(const K& key) {
  map.erase(key);
}
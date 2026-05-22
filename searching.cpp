#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <chrono>
#include <map>

using namespace std;
using namespace chrono;

struct FootballTeam {
    string country;
    string club_name;
    string city;
    int    year   = 0;
    string coach_fio;
    int    points = 0;

    FootballTeam() = default;
    FootballTeam(const string& country, const string& club_name,
                 const string& city, int year,
                 const string& coach_fio, int points)
        : country(country), club_name(club_name), city(city),
          year(year), coach_fio(coach_fio), points(points) {}
};

vector<FootballTeam> read_csv(size_t n) {

    ifstream file("football_teams.csv");
    vector<FootballTeam> data;

    if (!file.is_open()) {
        cerr << "Error: cannot open football_teams.csv" << endl;
        return data;
    }

    string line;
    getline(file, line);

    while (getline(file, line) && data.size() < n) {
        stringstream ss(line);
        string country, club_name, city, year, coach_fio, points;

        getline(ss, country, ';');
        getline(ss, club_name, ';');
        getline(ss, city, ';');
        getline(ss, year, ';');
        getline(ss, coach_fio, ';');
        getline(ss, points, ';');

        FootballTeam t;
        t.country = country;
        t.club_name = club_name;
        t.city = city;
        t.year = stoi(year);
        t.coach_fio = coach_fio;
        t.points = stoi(points);

        data.push_back(t);
    }

    return data;
}

// ================ ЛИНЕЙНЫЙ ПОИСК ===========================

vector<FootballTeam> linearSearch(const vector<FootballTeam>& arr, const string& key) {
    vector<FootballTeam> result;
    for (const auto& t : arr)
        if (t.country == key)
            result.push_back(t);
    return result;
}

// ============== БИНАРНОЕ ДЕРЕВО ПОИСКА ======================

struct BSTnode {
    FootballTeam data;
    BSTnode* left  = nullptr;
    BSTnode* right = nullptr;
    BSTnode(const FootballTeam& d) : data(d) {}
};

class BST {
public:
    BST() : root(nullptr) {}
    ~BST() { clear(root); }

    void add(const FootballTeam& t) {
        root = add(root, t);
    }

    vector<FootballTeam> search(const string& key) const {
        vector<FootballTeam> res;
        search(root, key, res);
        return res;
    }

private:
    BSTnode* root;

    BSTnode* add(BSTnode* node, const FootballTeam& t) {
        if (!node) return new BSTnode(t);
        if (t.country < node->data.country)
            node->left = add(node->left,  t);
        else
            node->right = add(node->right, t);
        return node;
    }

    void search(BSTnode* node, const string& key, vector<FootballTeam>& res) const {
        if (!node) return;
        if (key < node->data.country) {
            search(node->left, key, res);
        } else if (key > node->data.country) {
            search(node->right, key, res);
        } else {
            res.push_back(node->data);
            search(node->right, key, res);
        }
    }

    void clear(BSTnode* node) {
        if (!node) return;
        clear(node->left);
        clear(node->right);
        delete node;
    }
};

// ============ КРАСНО ЧЕРНОЕ ДЕРЕВО =========================

enum Color { RED, BLACK };

struct RBTnode {
    FootballTeam data;
    Color color = RED;
    RBTnode* left   = nullptr;
    RBTnode* right  = nullptr;
    RBTnode* parent = nullptr;
    RBTnode(const FootballTeam& d) : data(d) {}
};

class RBT {
public:
    RBT() { 
        nil = new RBTnode(FootballTeam());
        nil->color = BLACK;
        root = nil;
    }

    ~RBT() { clear(root); delete nil; }

    void add(const FootballTeam& t) {
        RBTnode* node = new RBTnode(t);
        node->left = node->right = node->parent = nil;
        node_add(node);
    }

    vector<FootballTeam> search(const string& key) const {
        vector<FootballTeam> res;
        search(root, key, res);
        return res;
    }

private:
    RBTnode* root;
    RBTnode* nil;

    void leftRotate(RBTnode* x) {
        RBTnode* y = x->right;
        x->right = y->left;
        if (y->left != nil) 
            y->left->parent = x;
        y->parent = x->parent;
        if (x->parent == nil)
            root = y;
        else if (x == x->parent->left) 
            x->parent->left = y;
        else    
            x->parent->right = y;
        y->left = x;
        x->parent = y;
    }

    void rightRotate(RBTnode* x) {
        RBTnode* y = x->left;
        x->left = y->right;
        if (y->right != nil) 
            y->right->parent = x;
        y->parent = x->parent;
        if (x->parent == nil)        
            root = y;
        else if (x == x->parent->right) 
            x->parent->right = y;
        else       
            x->parent->left = y;
        y->right  = x;
        x->parent = y;
    }

    void node_add(RBTnode* z) {
        RBTnode* y = nil;
        RBTnode* x = root;
        while (x != nil) {
            y = x;
            x = (z->data.country < x->data.country) ? x->left : x->right;
        }
        z->parent = y;
        if (y == nil)
            root = z;
        else if (z->data.country < y->data.country)
            y->left = z;
        else
            y->right = z;
        fix_add(z);
    }

    void fix_add(RBTnode* z) {
        while (z->parent->color == RED) {
            if (z->parent == z->parent->parent->left) {
                RBTnode* y = z->parent->parent->right;
                if (y->color == RED) {
                    z->parent->color = BLACK;
                    y->color = BLACK;
                    z->parent->parent->color = RED;
                    z = z->parent->parent;
                } else {
                    if (z == z->parent->right) {
                        z = z->parent;
                        leftRotate(z);
                    }
                    z->parent->color = BLACK;
                    z->parent->parent->color = RED;
                    rightRotate(z->parent->parent);
                }
            } else {
                RBTnode* y = z->parent->parent->left;
                if (y->color == RED) {
                    z->parent->color = BLACK;
                    y->color = BLACK;
                    z->parent->parent->color = RED;
                    z = z->parent->parent;
                } else {
                    if (z == z->parent->left) {
                        z = z->parent;
                        rightRotate(z);
                    }
                    z->parent->color = BLACK;
                    z->parent->parent->color = RED;
                    leftRotate(z->parent->parent);
                }
            }
        }
        root->color = BLACK;
    }

    void search(RBTnode* node, const string& key, vector<FootballTeam>& res) const {
        if (node == nil) return;
        if (key < node->data.country) {
            search(node->left, key, res);
        } else if (key > node->data.country) {
            search(node->right, key, res);
        } else {
            res.push_back(node->data);
            search(node->left,  key, res);
            search(node->right, key, res);
        }
    }

    void clear(RBTnode* node) {
        if (node == nil) return;
        clear(node->left);
        clear(node->right);
        delete node;
    }
};

// ============== ХЕШ ТАБЛИЦА ==============================

size_t h1(const string& s, size_t m) {
    const size_t P = 31;
    size_t hash = 0, p_pow = 1;
    for (unsigned char c : s) {
        hash  += (size_t)(c - 'a' + 1) * p_pow;
        p_pow *= P;
    }
    return hash % m;
}

size_t h2(const string& s, size_t m) {
    size_t b = 378551;
    size_t a = 63689;
    size_t hash = 0;
    for (unsigned char c : s) {
        hash = hash * a + c;
        a *= b;
    }
    return (hash % (m - 1)) + 1;
}

enum SlotState { EMPTY, OCCUPIED};

struct Slot {
    string key;
    vector<FootballTeam> values;
    SlotState state = EMPTY;
};

class HashTable {
public:
    
    HashTable(size_t capacity = 16)
        : capacity(capacity), size(0), hash_collisions(0)
    {
        table.resize(capacity);
    }

    void add(const FootballTeam& t) {
        if (size >= capacity * 7 / 10)
            rehash(capacity * 2);

        size_t a = h1(t.country, capacity);
        size_t b = h2(t.country, capacity);
        size_t idx = a;

        for (size_t i = 0; ; ++i) {
            SlotState st = table[idx].state;

            if (st == EMPTY) {
                table[idx].key = t.country;
                table[idx].state = OCCUPIED;
                table[idx].values.push_back(t);
                ++size;
                return;
            }

            if (st == OCCUPIED && table[idx].key == t.country) {
                table[idx].values.push_back(t);
                return;
            }

            idx = (a + (i + 1) * b) % capacity;
            ++hash_collisions;
        }
    }

    vector<FootballTeam> search(const string& key) const {
        size_t idx = probe(key);
        if (idx < capacity && table[idx].state == OCCUPIED && table[idx].key == key)
            return table[idx].values;
        return {};
    }
    size_t hashCollisions() const { return hash_collisions; }

private:
    size_t capacity;
    size_t size;
    size_t hash_collisions;
    vector<Slot> table;
    
    size_t probe(const string& key) const {
        size_t a = h1(key, capacity);
        size_t b = h2(key, capacity);
        size_t idx = a;

        for (size_t i = 1; i <= capacity; ++i) {
            SlotState st = table[idx].state;
            if (st == EMPTY) break;
            if (st == OCCUPIED && table[idx].key == key) return idx;
            idx = (a + i * b) % capacity;
        }
        return capacity;
    }

    void rehash(size_t new_capacity) {
        vector<Slot> old = std::move(table);
        capacity = new_capacity;
        table.assign(capacity, Slot{});
        size = 0;
        for (auto& slot : old)
            if (slot.state == OCCUPIED)
                for (auto& t : slot.values)
                    copy(t);
    }

    void copy(const FootballTeam& t) {
        size_t a = h1(t.country, capacity);
        size_t b = h2(t.country, capacity);
        size_t idx = a;

        for (size_t i = 1; ; ++i) {
            SlotState st = table[idx].state;
            if (st == EMPTY || (st == OCCUPIED && table[idx].key == t.country)) {
                if (st != OCCUPIED) {
                    table[idx].key   = t.country;
                    table[idx].state = OCCUPIED;
                    ++size;
                }
                table[idx].values.push_back(t);
                return;
            }
            idx = (a + i * b) % capacity;
        }
    }
};

// ============================================================

template<typename Func>
double times(Func&& func) {
    auto t0 = high_resolution_clock::now();
    func();
    auto t1 = high_resolution_clock::now();
    return duration<double, milli>(t1 - t0).count();
}

int main() {
    const vector<size_t> sizes = {
        100, 500, 1000, 5000, 10000, 50000,
        100000, 200000, 500000//, 1000000
    };

    const string SEARCH_KEY = "Russia";

    ofstream timingCSV("timings.csv");
    timingCSV << "Size;Linear;BST;RBT;HashTable;Multimap\n";

    ofstream collCSV("collisions.csv");
    collCSV << "Size;Collisions\n";

    cout << fixed;
    cout.precision(4);

    for (size_t n : sizes) {
        cout << "\nn = " << n << endl;

        vector<FootballTeam> data = read_csv(n);

        // Линейный поиск
        vector<FootballTeam> linearRes;
        double linear_search = times([&]{
            linearRes = linearSearch(data, SEARCH_KEY);
        });
        cout << "  Linear: " << linear_search << " ms  (found " << linearRes.size() << ")" << endl;

        // BST
        BST bst;
        for (const auto& t : data) bst.add(t);

        vector<FootballTeam> bstRes;
        double BST_search = times([&]{
            bstRes = bst.search(SEARCH_KEY);
        });
        cout << "  BST: " << BST_search << " ms  (found " << bstRes.size() << ")" << endl;

        // RBT
        RBT rbt;
        for (const auto& t : data) rbt.add(t);

        vector<FootballTeam> rbtRes;
        double RBT_search = times([&]{
            rbtRes = rbt.search(SEARCH_KEY);
        });
        cout << "  RBT: " << RBT_search << " ms  (found " << rbtRes.size() << ")" << endl;

        // Хеш-таблица
        HashTable ht(16);
        for (const auto& t : data) ht.add(t);

        vector<FootballTeam> htRes;
        double HT_search = times([&]{
            htRes = ht.search(SEARCH_KEY);
        });
        cout << "  HashTable: " << HT_search << " ms  (found " << htRes.size() 
        << ", hash_collisions =" << ht.hashCollisions() << ")" << endl;

        collCSV << n << ";" << ht.hashCollisions() << "\n";

        // std::multimap
        multimap<string, FootballTeam> mmap;
        for (const auto& t : data) mmap.insert({t.country, t});

        vector<FootballTeam> mmRes;
        size_t size;
        double MM_search = times([&]{
            auto range = mmap.equal_range(SEARCH_KEY);
            size = distance(range.first, range.second);
        });
        cout << "  Multimap: " << MM_search << " ms  (found " << size << ")" << endl;

        timingCSV << n << ";"
                  << linear_search << ";"
                  << BST_search << ";"
                  << RBT_search << ";"
                  << HT_search << ";"
                  << MM_search << "\n";
    }

    timingCSV.close();
    collCSV.close();
    return 0;
}
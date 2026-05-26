/**
 * @file football_search.cpp
 * @brief Сравнение алгоритмов поиска на данных футбольных команд.
 *
 * Реализованы и сравниваются по времени выполнения следующие структуры:
 * - Линейный поиск
 * - Бинарное дерево поиска (BST)
 * - Красно-чёрное дерево (RBT)
 * - Хеш-таблица с двойным хешированием
 * - std::multimap
 */

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <chrono>
#include <map>

using namespace std;
using namespace chrono;

// ============================================================

/**
 * @brief Структура, представляющая футбольную команду.
 */
struct FootballTeam {
    string country;    ///< Страна команды
    string club_name;  ///< Название клуба
    string city;       ///< Город базирования
    int    year  = 0;  ///< Год основания клуба
    string coach_fio;  ///< ФИО главного тренера
    int    points = 0; ///< Количество очков

    /// @brief Конструктор по умолчанию.
    FootballTeam() = default;

    /**
     * @brief Конструктор с параметрами.
     * @param country   Страна команды.
     * @param club_name Название клуба.
     * @param city      Город базирования.
     * @param year      Год основания.
     * @param coach_fio ФИО главного тренера.
     * @param points    Количество очков.
     */
    FootballTeam(const string& country, const string& club_name,
                 const string& city, int year,
                 const string& coach_fio, int points)
        : country(country), club_name(club_name), city(city),
          year(year), coach_fio(coach_fio), points(points) {}
};

// ============================================================

/**
 * @brief Читает первые @p n записей из файла football_teams.csv.
 *
 * Ожидаемый формат CSV: поля разделены символом ';', первая строка — заголовок.
 * Порядок полей: country; club_name; city; year; coach_fio; points.
 *
 * @param n Максимальное количество записей для чтения.
 * @return Вектор считанных объектов FootballTeam.
 *         Возвращает пустой вектор, если файл не удалось открыть.
 */
vector<FootballTeam> read_csv(size_t n) {

    ifstream file("football_teams.csv");
    vector<FootballTeam> data;

    if (!file.is_open()) {
        cerr << "Error: cannot open football_teams.csv" << endl;
        return data;
    }

    string line;
    getline(file, line); // пропустить заголовок

    while (getline(file, line) && data.size() < n) {
        stringstream ss(line);
        string country, club_name, city, year, coach_fio, points;

        getline(ss, country,   ';');
        getline(ss, club_name, ';');
        getline(ss, city,      ';');
        getline(ss, year,      ';');
        getline(ss, coach_fio, ';');
        getline(ss, points,    ';');

        FootballTeam t;
        t.country   = country;
        t.club_name = club_name;
        t.city      = city;
        t.year      = stoi(year);
        t.coach_fio = coach_fio;
        t.points    = stoi(points);

        data.push_back(t);
    }

    return data;
}

// ================ ЛИНЕЙНЫЙ ПОИСК ===========================

/**
 * @brief Линейный поиск команд по стране.
 *
 * Проходит по всем элементам массива и собирает совпадения.
 * Сложность: O(n).
 *
 * @param arr Вектор футбольных команд.
 * @param key Искомое название страны.
 * @return Вектор команд, у которых поле @c country совпадает с @p key.
 */
vector<FootballTeam> linearSearch(const vector<FootballTeam>& arr, const string& key) {
    vector<FootballTeam> result;
    for (const auto& t : arr)
        if (t.country == key)
            result.push_back(t);
    return result;
}

// ============== БИНАРНОЕ ДЕРЕВО ПОИСКА ======================

/**
 * @brief Узел бинарного дерева поиска.
 */
struct BSTnode {
    FootballTeam data;    ///< Данные, хранящиеся в узле
    BSTnode* left  = nullptr; ///< Левый потомок
    BSTnode* right = nullptr; ///< Правый потомок

    /**
     * @brief Конструктор узла.
     * @param d Данные для хранения в узле.
     */
    BSTnode(const FootballTeam& d) : data(d) {}
};

/**
 * @brief Бинарное дерево поиска (BST) по полю @c country.
 *
 * Поддерживает добавление элементов и поиск по ключу.
 * Ключом служит поле FootballTeam::country.
 * При дублирующихся ключах новые узлы уходят в правое поддерево.
 * Сложность поиска в среднем: O(log n), в худшем: O(n).
 */
class BST {
public:
    /// @brief Конструктор по умолчанию. Создаёт пустое дерево.
    BST() : root(nullptr) {}

    /// @brief Деструктор. Освобождает всю память дерева.
    ~BST() { clear(root); }

    /**
     * @brief Добавляет команду в дерево.
     * @param t Команда для добавления.
     */
    void add(const FootballTeam& t) {
        root = add(root, t);
    }

    /**
     * @brief Ищет все команды с заданным полем @c country.
     * @param key Искомое название страны.
     * @return Вектор найденных команд.
     */
    vector<FootballTeam> search(const string& key) const {
        vector<FootballTeam> res;
        search(root, key, res);
        return res;
    }

private:
    BSTnode* root; ///< Корень дерева

    /**
     * @brief Рекурсивно добавляет узел в поддерево.
     * @param node Текущий корень поддерева.
     * @param t    Добавляемая команда.
     * @return Новый корень поддерева.
     */
    BSTnode* add(BSTnode* node, const FootballTeam& t) {
        if (!node) return new BSTnode(t);
        if (t.country < node->data.country)
            node->left  = add(node->left,  t);
        else
            node->right = add(node->right, t);
        return node;
    }

    /**
     * @brief Рекурсивно ищет все узлы с заданным ключом.
     * @param node Текущий узел поддерева.
     * @param key  Искомое название страны.
     * @param res  Вектор для накопления результатов.
     */
    void search(BSTnode* node, const string& key, vector<FootballTeam>& res) const {
        if (!node) return;
        if (key < node->data.country) {
            search(node->left, key, res);
        } else if (key > node->data.country) {
            search(node->right, key, res);
        } else {
            res.push_back(node->data);
            search(node->right, key, res); // дубли уходят вправо
        }
    }

    /**
     * @brief Рекурсивно удаляет всё поддерево.
     * @param node Корень удаляемого поддерева.
     */
    void clear(BSTnode* node) {
        if (!node) return;
        clear(node->left);
        clear(node->right);
        delete node;
    }
};

// ============ КРАСНО-ЧЁРНОЕ ДЕРЕВО =========================

/**
 * @brief Цвет узла красно-чёрного дерева.
 */
enum Color { RED, BLACK };

/**
 * @brief Узел красно-чёрного дерева.
 */
struct RBTnode {
    FootballTeam data;          ///< Данные узла
    Color    color  = RED;      ///< Цвет узла (RED / BLACK)
    RBTnode* left   = nullptr;  ///< Левый потомок
    RBTnode* right  = nullptr;  ///< Правый потомок
    RBTnode* parent = nullptr;  ///< Родительский узел

    /**
     * @brief Конструктор узла.
     * @param d Данные для хранения.
     */
    RBTnode(const FootballTeam& d) : data(d) {}
};

/**
 * @brief Красно-чёрное дерево (RBT) по полю @c country.
 *
 * Самобалансирующееся BST, гарантирующее высоту O(log n).
 * Сложность поиска, вставки и удаления: O(log n).
 * Использует фиктивный nil-узел (sentinel) вместо nullptr.
 */
class RBT {
public:
    /**
     * @brief Конструктор. Инициализирует дерево с sentinel-узлом.
     */
    RBT() {
        nil = new RBTnode(FootballTeam());
        nil->color = BLACK;
        root = nil;
    }

    /// @brief Деструктор. Освобождает все узлы и sentinel.
    ~RBT() { clear(root); delete nil; }

    /**
     * @brief Добавляет команду в дерево.
     * @param t Команда для добавления.
     */
    void add(const FootballTeam& t) {
        RBTnode* node = new RBTnode(t);
        node->left = node->right = node->parent = nil;
        node_add(node);
    }

    /**
     * @brief Ищет все команды с заданным полем @c country.
     * @param key Искомое название страны.
     * @return Вектор найденных команд.
     */
    vector<FootballTeam> search(const string& key) const {
        vector<FootballTeam> res;
        search(root, key, res);
        return res;
    }

private:
    RBTnode* root; ///< Корень дерева
    RBTnode* nil;  ///< Sentinel-узел (листья и "нулевой" родитель корня)

    /**
     * @brief Левый поворот вокруг узла @p x.
     * @param x Узел, вокруг которого выполняется поворот.
     */
    void leftRotate(RBTnode* x) {
        RBTnode* y = x->right;
        x->right = y->left;
        if (y->left != nil)
            y->left->parent = x;
        y->parent = x->parent;
        if (x->parent == nil)
            root = y;
        else if (x == x->parent->left)
            x->parent->left  = y;
        else
            x->parent->right = y;
        y->left   = x;
        x->parent = y;
    }

    /**
     * @brief Правый поворот вокруг узла @p x.
     * @param x Узел, вокруг которого выполняется поворот.
     */
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
            x->parent->left  = y;
        y->right  = x;
        x->parent = y;
    }

    /**
     * @brief Вставляет новый узел @p z в дерево (без балансировки).
     * @param z Вставляемый узел.
     */
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
            y->left  = z;
        else
            y->right = z;
        fix_add(z);
    }

    /**
     * @brief Восстанавливает свойства красно-чёрного дерева после вставки.
     * @param z Только что вставленный узел.
     */
    void fix_add(RBTnode* z) {
        while (z->parent->color == RED) {
            if (z->parent == z->parent->parent->left) {
                RBTnode* y = z->parent->parent->right; // дядя
                if (y->color == RED) {
                    // Случай 1: дядя красный — перекраска
                    z->parent->color          = BLACK;
                    y->color                  = BLACK;
                    z->parent->parent->color  = RED;
                    z = z->parent->parent;
                } else {
                    if (z == z->parent->right) {
                        // Случай 2: дядя чёрный, z — правый потомок
                        z = z->parent;
                        leftRotate(z);
                    }
                    // Случай 3: дядя чёрный, z — левый потомок
                    z->parent->color         = BLACK;
                    z->parent->parent->color = RED;
                    rightRotate(z->parent->parent);
                }
            } else {
                RBTnode* y = z->parent->parent->left; // дядя (симметрия)
                if (y->color == RED) {
                    z->parent->color         = BLACK;
                    y->color                 = BLACK;
                    z->parent->parent->color = RED;
                    z = z->parent->parent;
                } else {
                    if (z == z->parent->left) {
                        z = z->parent;
                        rightRotate(z);
                    }
                    z->parent->color         = BLACK;
                    z->parent->parent->color = RED;
                    leftRotate(z->parent->parent);
                }
            }
        }
        root->color = BLACK;
    }

    /**
     * @brief Рекурсивно ищет все узлы с заданным ключом.
     * @param node Текущий узел поддерева.
     * @param key  Искомое название страны.
     * @param res  Вектор для накопления результатов.
     */
    void search(RBTnode* node, const string& key, vector<FootballTeam>& res) const {
        if (node == nil) return;
        if (key < node->data.country) {
            search(node->left, key, res);
        } else if (key > node->data.country) {
            search(node->right, key, res);
        } else {
            res.push_back(node->data);
            search(node->left,  key, res); // возможны дубли в обоих поддеревьях
            search(node->right, key, res);
        }
    }

    /**
     * @brief Рекурсивно удаляет всё поддерево (кроме sentinel).
     * @param node Корень удаляемого поддерева.
     */
    void clear(RBTnode* node) {
        if (node == nil) return;
        clear(node->left);
        clear(node->right);
        delete node;
    }
};

// ============== ХЕШ-ТАБЛИЦА ==============================

/**
 * @brief Первая хеш-функция (полиномиальный хеш).
 *
 * Вычисляет хеш строки с основанием P=31.
 *
 * @param s Входная строка.
 * @param m Размер таблицы (делитель).
 * @return Значение хеша в диапазоне [0, m).
 */
size_t h1(const string& s, size_t m) {
    const size_t P = 31;
    size_t hash = 0, p_pow = 1;
    for (unsigned char c : s) {
        hash  += (size_t)(c - 'a' + 1) * p_pow;
        p_pow *= P;
    }
    return hash % m;
}

/**
 * @brief Вторая хеш-функция для двойного хеширования.
 *
 * Используется в качестве шага зондирования при разрешении коллизий.
 * Гарантирует ненулевой результат (диапазон [1, m-1]).
 *
 * @param s Входная строка.
 * @param m Размер таблицы (делитель).
 * @return Шаг зондирования в диапазоне [1, m-1].
 */
size_t h2(const string& s, size_t m) {
    size_t b    = 378551;
    size_t a    = 63689;
    size_t hash = 0;
    for (unsigned char c : s) {
        hash = hash * a + c;
        a   *= b;
    }
    return (hash % (m - 1)) + 1;
}

/**
 * @brief Состояние ячейки хеш-таблицы.
 */
enum SlotState { EMPTY, OCCUPIED };

/**
 * @brief Ячейка хеш-таблицы.
 *
 * Хранит ключ и список всех команд с данным ключом (страной).
 */
struct Slot {
    string               key;            ///< Ключ ячейки (название страны)
    vector<FootballTeam> values;         ///< Список команд с данным ключом
    SlotState            state = EMPTY;  ///< Состояние ячейки
};

/**
 * @brief Хеш-таблица с открытой адресацией и двойным хешированием.
 *
 * Коллизии разрешаются методом двойного хеширования:
 * @code
 *   idx = (h1(key) + i * h2(key)) % capacity
 * @endcode
 * При заполнении более 70% выполняется удвоение ёмкости (рехеширование).
 * Дублирующиеся ключи хранятся в списке values одной ячейки.
 */
class HashTable {
public:
    /**
     * @brief Конструктор.
     * @param capacity Начальная ёмкость таблицы (по умолчанию 16).
     */
    HashTable(size_t capacity = 16)
        : capacity(capacity), size(0), hash_collisions(0)
    {
        table.resize(capacity);
    }

    /**
     * @brief Добавляет команду в таблицу.
     *
     * Если таблица заполнена более чем на 70%, выполняется рехеширование.
     * Команды с одинаковым полем @c country попадают в один слот.
     *
     * @param t Добавляемая команда.
     */
    void add(const FootballTeam& t) {
        if (size >= capacity * 7 / 10)
            rehash(capacity * 2);

        size_t a   = h1(t.country, capacity);
        size_t b   = h2(t.country, capacity);
        size_t idx = a;

        for (size_t i = 0; ; ++i) {
            SlotState st = table[idx].state;

            if (st == EMPTY) {
                table[idx].key   = t.country;
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

    /**
     * @brief Ищет все команды с заданным полем @c country.
     * @param key Искомое название страны.
     * @return Вектор найденных команд (пустой, если ключ не найден).
     */
    vector<FootballTeam> search(const string& key) const {
        size_t idx = probe(key);
        if (idx < capacity && table[idx].state == OCCUPIED && table[idx].key == key)
            return table[idx].values;
        return {};
    }

    /**
     * @brief Возвращает суммарное число коллизий за всё время работы таблицы.
     * @return Количество коллизий.
     */
    size_t hashCollisions() const { return hash_collisions; }

private:
    size_t      capacity;        ///< Текущая ёмкость таблицы
    size_t      size;            ///< Число занятых слотов
    size_t      hash_collisions; ///< Счётчик коллизий
    vector<Slot> table;          ///< Массив ячеек

    /**
     * @brief Ищет индекс ячейки с заданным ключом (метод зондирования).
     * @param key Искомый ключ.
     * @return Индекс найденной ячейки или @c capacity, если ключ не найден.
     */
    size_t probe(const string& key) const {
        size_t a   = h1(key, capacity);
        size_t b   = h2(key, capacity);
        size_t idx = a;

        for (size_t i = 1; i <= capacity; ++i) {
            SlotState st = table[idx].state;
            if (st == EMPTY) break;
            if (st == OCCUPIED && table[idx].key == key) return idx;
            idx = (a + i * b) % capacity;
        }
        return capacity; // не найдено
    }

    /**
     * @brief Увеличивает ёмкость таблицы и перераспределяет все элементы.
     * @param new_capacity Новая ёмкость (обычно удвоенная текущая).
     */
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

    /**
     * @brief Вспомогательный метод вставки при рехешировании.
     *
     * Аналогичен @c add(), но без проверки порога заполнения.
     *
     * @param t Команда для копирования в новую таблицу.
     */
    void copy(const FootballTeam& t) {
        size_t a   = h1(t.country, capacity);
        size_t b   = h2(t.country, capacity);
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

/**
 * @brief Измеряет время выполнения произвольной функции.
 *
 * @tparam Func Тип вызываемого объекта (лямбда, функция и т.д.).
 * @param func  Функция без аргументов и возвращаемого значения.
 * @return Время выполнения в миллисекундах.
 */
template<typename Func>
double times(Func&& func) {
    auto t0 = high_resolution_clock::now();
    func();
    auto t1 = high_resolution_clock::now();
    return duration<double, milli>(t1 - t0).count();
}

/**
 * @brief Точка входа программы.
 *
 * Для каждого размера выборки из вектора @c sizes:
 * -# Читает данные из CSV.
 * -# Строит все структуры данных и измеряет время поиска ключа @c SEARCH_KEY.
 * -# Выводит результаты в консоль.
 * -# Записывает тайминги в @c timings.csv и число коллизий в @c collisions.csv.
 *
 * @return 0 при успешном завершении.
 */
int main() {
    const vector<size_t> sizes = {
        100, 500, 1000, 5000, 10000, 50000,
        100000, 200000, 500000//, 1000000
    };

    const string SEARCH_KEY = "Russia"; ///< Ключ поиска во всех структурах

    ofstream timingCSV("timings.csv");
    timingCSV << "Size;Linear;BST;RBT;HashTable;Multimap\n";

    ofstream collCSV("collisions.csv");
    collCSV << "Size;Collisions\n";

    cout << fixed;
    cout.precision(4);

    for (size_t n : sizes) {
        cout << "\nn = " << n << endl;

        vector<FootballTeam> data = read_csv(n);

        // --- Линейный поиск ---
        vector<FootballTeam> linearRes;
        double linear_search = times([&]{
            linearRes = linearSearch(data, SEARCH_KEY);
        });
        cout << "  Linear: " << linear_search << " ms  (found " << linearRes.size() << ")" << endl;

        // --- BST ---
        BST bst;
        for (const auto& t : data) bst.add(t);

        vector<FootballTeam> bstRes;
        double BST_search = times([&]{
            bstRes = bst.search(SEARCH_KEY);
        });
        cout << "  BST: " << BST_search << " ms  (found " << bstRes.size() << ")" << endl;

        // --- RBT ---
        RBT rbt;
        for (const auto& t : data) rbt.add(t);

        vector<FootballTeam> rbtRes;
        double RBT_search = times([&]{
            rbtRes = rbt.search(SEARCH_KEY);
        });
        cout << "  RBT: " << RBT_search << " ms  (found " << rbtRes.size() << ")" << endl;

        // --- Хеш-таблица ---
        HashTable ht(16);
        for (const auto& t : data) ht.add(t);

        vector<FootballTeam> htRes;
        double HT_search = times([&]{
            htRes = ht.search(SEARCH_KEY);
        });
        cout << "  HashTable: " << HT_search << " ms  (found " << htRes.size()
             << ", hash_collisions =" << ht.hashCollisions() << ")" << endl;

        collCSV << n << ";" << ht.hashCollisions() << "\n";

        // --- std::multimap ---
        multimap<string, FootballTeam> mmap;
        for (const auto& t : data) mmap.insert({t.country, t});

        size_t mmSize;
        double MM_search = times([&]{
            auto range = mmap.equal_range(SEARCH_KEY);
            mmSize = distance(range.first, range.second);
        });
        cout << "  Multimap: " << MM_search << " ms  (found " << mmSize << ")" << endl;

        timingCSV << n         << ";"
                  << linear_search << ";"
                  << BST_search    << ";"
                  << RBT_search    << ";"
                  << HT_search     << ";"
                  << MM_search     << "\n";
    }

    timingCSV.close();
    collCSV.close();
    return 0;
}
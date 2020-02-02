#include <iostream>
#include <memory>
#include <string>
#include <vector>
/*


В базе данных хранится N записей, вида (Name, a1, a2, … , ak) — во всех записях
одинаковое число параметров. На вход задачи подаётся приоритет полей —
перестановка на числах 1,...,k — записи нужно вывести в соответствии с этим
приоритетом. В случае, если приоритет полей таков: 3 4 2 1, то это следует
воспринимать так: надо читать как: приоритет значений из 3 колонки самый
высокий, приоритет значений из колонки 4 ниже, приоритет значений из колонки 2
ещё ниже, а приоритет значений из колонки 1 самый низкий. Формат ввода

N (1⩽N⩽1000) k (1⩽k;⩽10) p1 p2 ... pk (перестановка на k числах, разделитель
пробел) N строк вида Name a1 a2 ... ak (разделитель — пробел) Формат вывода

N строк с именами согласно приоритету

*/

struct Field {
  Field() {}
  Field(std::string name, std::vector<int>&& values)
      : name(name), values(std::move(values)) {}

  std::string name;
  std::vector<int> values;
};

class PriorityComparator {
 public:
  PriorityComparator(const std::vector<int>& priorities)
      : priorities(priorities) {}
  bool operator()(const Field& l, const Field& r) const;

 private:
  const std::vector<int> priorities;
};

template <class T, class TComparator>
void insertionSort(T* arr, int size, const TComparator& compareLess) {
  for (int i = 0; i < size - 1; ++i) {
    int j = i + 1;
    T val = arr[j];

    while (j > 0 && compareLess(val, arr[j - 1])) {
      arr[j] = std::move(arr[j - 1]);
      --j;
    }

    arr[j] = std::move(val);
  }
}

int main() {
  int n, k;
  std::cin >> n >> k;
  std::vector<int> priorities(k);
  std::vector<Field> fields;

  for (auto& p : priorities) std::cin >> p;

  for (int i = 0; i < n; ++i) {
    std::string name;
    std::cin >> name;
    std::vector<int> values(k);

    for (auto& v : values) std::cin >> v;

    fields.emplace_back(name, std::move(values));
  }

  PriorityComparator comparator(priorities);
  insertionSort(fields.data(), fields.size(), comparator);

  for (const auto& f : fields) std::cout << f.name << '\n';
}

bool PriorityComparator::operator()(const Field& l, const Field& r) const {
  for (const auto& index : priorities) {
    if (l.values[index - 1] > r.values[index - 1])
      return true;
    else if (r.values[index - 1] > l.values[index - 1])
      return false;
  }

  return false;
}

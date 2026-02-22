#include <iostream>

#include "core/Array.h"

static int log2i(int x) {
    int p = 0;
    while (x > 1) { x >>= 1; ++p; }
    return p;
}

// TIP To <b>Run</b> code, press <shortcut actionId="Run"/> or click the <icon src="AllIcons.Actions.Execute"/> icon in the gutter.
int main() {

    Array<int> a;
    for (int i = 0; i < 10; ++i)
        a.insert(i + 1);

    for (int i = 0; i < a.size(); ++i)
        a[i] *= 2;

    for (auto it = a.iterator(); it.hasNext(); it.next()){
        std::cout << it.get() << std::endl;
    }

    Array<int> a1;

    int prevCap = a1.capacity();


    std::cout << "n | size | capacity | log2(cap) graph\n";
    std::cout << "-------------------------------------\n";

    for (int i = 0; i <= 2000; ++i) {
        if (i > 0) a1.insert(i);

        int cap = a1.capacity();
        if (i == 0 || cap != prevCap) {
            int bars = log2i(cap);
            std::cout << i << " | " << a1.size() << " | " << cap << " | ";
            for (int j = 0; j < bars; ++j) std::cout << "#";
            std::cout << "  <-- realloc\n";
            prevCap = cap;
        }
    }
    return 0;
}
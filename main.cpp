#include <iostream>

#include "core/Array.h"

// TIP To <b>Run</b> code, press <shortcut actionId="Run"/> or click the <icon src="AllIcons.Actions.Execute"/> icon in the gutter.
int main() {
    // TIP Press <shortcut actionId="RenameElement"/> when your caret is at the <b>lang</b> variable name to see how CLion can help you rename it.
    Array<int> a;
    for (int i = 0; i < 10; ++i)
        a.insert(i + 1);

    for (int i = 0; i < a.size(); ++i)
        a[i] *= 2;

    for (auto it = a.iterator(); it.hasNext(); it.next()){
        std::cout << it.get() << std::endl;
    }

    return 0;
}
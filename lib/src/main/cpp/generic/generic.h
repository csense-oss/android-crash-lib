#include <csignal>
#include <memory>
#include <cxxabi.h>

#ifndef GENERIC_H
#define GENERIC_H


int removeSigaction(int signal, const struct sigaction *action);

template<class InputIterator, class Size, class Function>
InputIterator for_each_indexed_n(InputIterator first, Size orig_n, Function action) {
    Size index = 0;
    while (index < orig_n) {
        action(index, *first);
        first++;
        index++;
    }
    return first;
}


const char *createCrashMessage(siginfo *siginfo);

#endif //GENERIC_H


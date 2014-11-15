#include <cctype>

#include "sexp.h"


SexpPtr Sexpressionizer::getNextSexp() {
    std::ws(m_input);
    char next = m_input.peek();

    switch (next) {
    case '(':
        return getNextList();
        break;

    case ')':
        throw std::make_shared<ParseException>("Encountered unexpected close"
                                               " parentheses.");
        break;

    default:
        return getNextAtom();
        break;
    }

}

bool is_symbol_char(char c) {
    // Symbols/atoms are anything that is not whitespace or a paren
    return ((! std::isspace(c))
            && (c != ')')
            && (c != '('));
}

SexpPtr Sexpressionizer::getNextAtom() {

    std::string symbol;

    char next = m_input.get();
    while (! m_input.eof()  && is_symbol_char(next)) {
        symbol.push_back(next);
        next = m_input.get();
    }

    return std::dynamic_pointer_cast<Sexp>(std::make_shared<Atom>(symbol));
}

SexpPtr Sexpressionizer::getNextList() {

    ListPtr list(new List());

    // Eat the open paren
    m_input.get();

    char next = m_input.peek();
    while (true) {
        if (m_input.eof()) {
            throw std::make_shared<ParseException>("End of file encountered before"
                                                   " close paren was found.");
        }

        switch (next) {

        // Eat whitespace
        case ' ':
            std::ws(m_input);
            next = m_input.peek();
            break;

        // End of the list
        case ')':
            return std::dynamic_pointer_cast<Sexp>(list);
            break;

        default:
            list->addSexp(getNextSexp());
        } // End switch

    } // End while


}

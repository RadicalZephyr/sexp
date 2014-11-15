#include <cctype>

#include "sexp.h"

char Sexpressionizer::skip_ws() {
    char next = m_input.peek();;
    while (m_input) {
        if (!std::isspace(next)) {
            return next;
        }
        m_input.get();
        next = m_input.peek();
    }
    return -1;
}

SexpPtr Sexpressionizer::getNextSexp() {

    char next = skip_ws();

    // Eat leading whitespace a different way.t
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
    while (m_input  && is_symbol_char(next)) {
        symbol.push_back(next);
        next = m_input.get();
    }

    if (m_input) {
        // Put back the last character (which was not a symbol)
        m_input.unget();
    }
    return std::dynamic_pointer_cast<Sexp>(std::make_shared<Atom>(symbol));
}

SexpPtr Sexpressionizer::getNextList() {

    ListPtr list(new List());

    // Eat the open paren
    m_input.get();

    char next = m_input.peek();
    while (true) {
        if (!m_input) {
            throw std::make_shared<ParseException>("End of file encountered before"
                                                   " close paren was found.");
        }

        next = skip_ws();

        switch (next) {

        // End of the list
        case ')':
            // Eat the end paren for this list to avoid closing
            // multiple lists
            m_input.get();
            return std::dynamic_pointer_cast<Sexp>(list);
            break;

        default:
            list->addSexp(getNextSexp());
            next = m_input.peek();
            break;
        } // End switch

    } // End while


}

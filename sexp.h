// Lexer for s-expressions
#pragma once
#include <iostream>
#include <exception>
#include <memory>
#include <vector>
#include <string>

class Sexp {

};

typedef std::shared_ptr<Sexp> SexpPtr;

typedef std::vector<SexpPtr> SexpVector;


class List: public Sexp {
    SexpVector m_sexps;

public:

    void addSexp(SexpPtr sexp) {
        m_sexps.push_back(sexp);
    }

};

typedef std::shared_ptr<List> ListPtr;


class Atom: public Sexp {
    std::string m_text;

public:
    Atom(std::string text): m_text(text) {};

};

typedef std::shared_ptr<Atom> AtomPtr;


class ParseException: public std::exception
{
    const char *m_what;

public:
    virtual const char *what() const throw()
    {
        return m_what;
    }

public:

    ParseException(const char  *what): m_what(what) {};

};

typedef std::shared_ptr<ParseException> ParseExceptionPtr;


class Sexpressionizer {

    std::istream &m_input;

public:

    Sexpressionizer(std::istream &input): m_input(input) {};

    SexpPtr getNextSexp();

    SexpPtr getNextAtom();

    SexpPtr getNextList();
};

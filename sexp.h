// Lexer for s-expressions
#pragma once
#include <iostream>
#include <exception>
#include <memory>
#include <vector>
#include <string>

class Visitor;

class Sexp {
public:
    virtual void print(void) = 0;

    virtual void accept(Visitor &v) = 0;
};

typedef std::shared_ptr<Sexp> SexpPtr;

typedef std::vector<SexpPtr> SexpVector;


class List: public Sexp {
    SexpVector m_sexps;

public:
    virtual void print(void) {
        std::cout << "(";
        std::string sep = "";
        for (SexpVector::iterator itr = m_sexps.begin();
             itr < m_sexps.end(); itr++) {
            std::cout << sep;
            (*itr)->print();
            sep = " ";
        }

        std::cout << ")";
    }

    virtual void accept(class Visitor &v);

    void addSexp(SexpPtr sexp) {
        m_sexps.push_back(sexp);
    }

};

typedef std::shared_ptr<List> ListPtr;


class Atom: public Sexp {
    std::string m_text;

public:

    virtual void print(void) {
        std::cout << m_text;
    }

    virtual void accept(class Visitor &v);

    Atom(std::string text): m_text(text) {};

};

typedef std::shared_ptr<Atom> AtomPtr;



class Visitor {

 public:
    virtual void visit(List *e) = 0;
    virtual void visit(Atom *e) = 0;

};



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

    char skip_ws();

public:

    Sexpressionizer(std::istream &input): m_input(input) {};

    SexpPtr getNextSexp();

    SexpPtr getNextAtom();

    SexpPtr getNextList();
};

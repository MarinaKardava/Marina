#ifndef LEXER_H
#define LEXER_H

#include "../include/abstract_scaner.h"
#include "../include/error_count.h"
#include "../include/location.h"
#include <string>"

enum lexemcodes : unsigned short {
    None,             Unknown,     Colon,           
    Semicolon,        Comma,       Cond_op,         
    Lor,              LNor,        LXor,            
    LNXor,            LAnd,        LNAnd,           
    LNot,             LT,          GT,              
    LEQ,              GEQ,         Equal,           
    Nequal,           Bor,         BNor,            
    BXor,             BNXor,       BAnd,            
    BNAnd,            LShift,      RShift,          
    BNot,             Plus,        Minus,           
    Dim_size,         Open_round,  Close_round,     
    Open_square,      Close_square,Open_cur_bracket,
    Close_cur_bracket,After_label, Assign,          
    Mul,              Div,         Module,          
    FDiv,             Pow,         FPow,            
    PlusAssign,       MinusAssign, MultAssign,      
    DivAssign,        FDivAssign,  PowAssign,       
    FPowAssign,       ModuleAssign,BAndAssign,      
    BNAndAssign,      LShiftAssign,RShiftAssign,    
    BorAssign,        BNorAssign,  BXorAssign,      
    BNXorAssign,      LorAssign,   LNorAssign,      
    LXorAssign,       LNXorAssign, LAndAssign,      
    LNAndAssign,      String,      If,              
    Float,            Else,        Var,             
    Int,              Bool,        Char,            
    Void,             Array,       True,            
    False,            Proto,       Func,            
    Const,            Elif,        Endif,           
    For,              In,          While,           
    Repeat,           Until,       Exit,            
    Continue,         Return,      Read,            
    Print,            Then,        Kw_bool,         
    Kw_string,        Kw_float,    Kw_int,          
    Kw_char,          Kw_void
};

struct Lexem_info{
    lexemcodes code;
    union{
        size_t    ident_index;
        size_t    string_index;
        char32_t  c;
unsigned __int128 int_val; __float128 float_val;
    };
};

class Lexer : public Abstract_scaner<Lexem_info> {
public:
    Lexer() = default;
    Lexer(Location* location, const Errors_and_tries& et) :
        Abstract_scaner<Lexem_info>(location, et) {};
    Lexer(const Lexer& orig) = default;
    virtual ~Lexer() = default;
    virtual Lexem_info current_lexem();
private:
    enum Automaton_name{
        A_start,     A_unknown, A_idKeyword, 
        A_delimiter, A_number,  A_string
    };
    Automaton_name automaton; /* current automaton */

   typedef bool (Lexer::*Automaton_proc)();
    /* This is the type of pointer to the member function that implements the
     * automaton that processes the lexeme. This function must return true if
     * the lexeme is not yet parsed, and false otherwise. */

    typedef void (Lexer::*Final_proc)();
    /* And this is the type of the pointer to the member function that performs
     * the necessary actions in the event of an unexpected end of the lexeme. */

    static Automaton_proc procs[];
    static Final_proc     finals[];

    /* Lexeme processing functions: */
    bool start_proc();     bool unknown_proc();   
    bool idkeyword_proc(); bool delimiter_proc(); 
    bool number_proc();    bool string_proc();
    /* functions for performing actions in case of an
     * unexpected end of the token */
    void none_proc();            void unknown_final_proc();   
    void idkeyword_final_proc(); void delimiter_final_proc(); 
    void number_final_proc();    void string_final_proc();
};
#endif

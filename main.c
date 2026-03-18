#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// --- ENUM DE TIPOS DE TOKEN ---
typedef enum {
    TOKEN_INVALID,
    TOKEN_INPUT,
    TOKEN_IDENTIFIER,
    TOKEN_NUMBER,
    TOKEN_STRING,
    TOKEN_PLUS,
    TOKEN_MINUS,
    TOKEN_MULTIPLY,
    TOKEN_DIVIDE,
    TOKEN_MODULO,
    TOKEN_ASSIGN,
    TOKEN_EQUAL,
    TOKEN_NOT_EQUAL,
    TOKEN_LESS,
    TOKEN_LESS_EQUAL,
    TOKEN_GREATER,
    TOKEN_GREATER_EQUAL,
    TOKEN_AND,
    TOKEN_OR,
    TOKEN_NOT,
    TOKEN_LPAREN,
    TOKEN_RPAREN,
    TOKEN_LBRACE,
    TOKEN_RBRACE,
    TOKEN_LBRACKET,
    TOKEN_RBRACKET,
    TOKEN_COMMA,
    TOKEN_COLON,
    TOKEN_SEMICOLON,
    TOKEN_DOT,
    TOKEN_RESERVED_VAR,
    TOKEN_RESERVED_CONST,
    TOKEN_RESERVED_IF,
    TOKEN_RESERVED_ELSE,
    TOKEN_RESERVED_ELIF,
    TOKEN_RESERVED_FOR,
    TOKEN_RESERVED_IN,
    TOKEN_RESERVED_RANGE,
    TOKEN_RESERVED_FUNC,
    TOKEN_RESERVED_RETURN,
    TOKEN_RESERVED_PRINT,
    TOKEN_RESERVED_INT,
    TOKEN_RESERVED_FLOAT,
    TOKEN_RESERVED_STR,
    TOKEN_RESERVED_BOOL,
    TOKEN_RESERVED_UNDEFINED,
    TOKEN_RESERVED_NUM,
    TOKEN_RESERVED_LENGTH,
    TOKEN_RESERVED_INPUT,
    TOKEN_RESERVED_WHILE,
    TOKEN_RESERVED_ARRAY,
    TOKEN_COMMENT,
    TOKEN_EOF
} TokenType;

// --- STRUCT DE TOKEN ---
typedef struct {
    TokenType type;
    char lexeme[128];
    char literal[128]; // Solo para strings y números
    int line;
    int column;
} Token;

// --- MAPEO DE TIPOS DE TOKEN A STRING ---
const char* token_type_to_string(TokenType type) {
    switch(type) {
        case TOKEN_IDENTIFIER: return "IDENTIFIER";
        case TOKEN_NUMBER: return "NUMBER";
        case TOKEN_STRING: return "STRING";
        case TOKEN_PLUS: return "PLUS";
        case TOKEN_MINUS: return "MINUS";
        case TOKEN_MULTIPLY: return "MULTIPLY";
        case TOKEN_DIVIDE: return "DIVIDE";
        case TOKEN_MODULO: return "MODULO";
        case TOKEN_ASSIGN: return "ASSIGN";
        case TOKEN_EQUAL: return "EQUAL";
        case TOKEN_NOT_EQUAL: return "NOT_EQUAL";
        case TOKEN_LESS: return "LESS";
        case TOKEN_LESS_EQUAL: return "LESS_EQUAL";
        case TOKEN_GREATER: return "GREATER";
        case TOKEN_GREATER_EQUAL: return "GREATER_EQUAL";
        case TOKEN_AND: return "AND";
        case TOKEN_OR: return "OR";
        case TOKEN_NOT: return "NOT";
        case TOKEN_LPAREN: return "LPAREN";
        case TOKEN_RPAREN: return "RPAREN";
        case TOKEN_LBRACE: return "LBRACE";
        case TOKEN_RBRACE: return "RBRACE";
        case TOKEN_LBRACKET: return "LBRACKET";
        case TOKEN_RBRACKET: return "RBRACKET";
        case TOKEN_COMMA: return "COMMA";
        case TOKEN_COLON: return "COLON";
        case TOKEN_SEMICOLON: return "SEMICOLON";
        case TOKEN_DOT: return "DOT";
        case TOKEN_RESERVED_VAR: return "VAR";
        case TOKEN_RESERVED_CONST: return "CONST";
        case TOKEN_RESERVED_IF: return "IF";
        case TOKEN_RESERVED_ELSE: return "ELSE";
        case TOKEN_RESERVED_ELIF: return "ELIF";
        case TOKEN_RESERVED_FOR: return "FOR";
        case TOKEN_RESERVED_IN: return "IN";
        case TOKEN_RESERVED_RANGE: return "RANGE";
        case TOKEN_RESERVED_FUNC: return "FUNC";
        case TOKEN_RESERVED_RETURN: return "RETURN";
        case TOKEN_RESERVED_PRINT: return "PRINT";
        case TOKEN_RESERVED_INT: return "INT";
        case TOKEN_RESERVED_FLOAT: return "FLOAT";
        case TOKEN_RESERVED_STR: return "STR";
        case TOKEN_RESERVED_BOOL: return "BOOL";
        case TOKEN_RESERVED_UNDEFINED: return "UNDEFINED";
        case TOKEN_RESERVED_NUM: return "NUM";
        case TOKEN_RESERVED_LENGTH: return "LENGTH";
        case TOKEN_RESERVED_INPUT: return "INPUT";
        case TOKEN_RESERVED_WHILE: return "WHILE";
        case TOKEN_RESERVED_ARRAY: return "ARRAY";
        case TOKEN_COMMENT: return "COMMENT";
        case TOKEN_EOF: return "EOF";
        case TOKEN_INPUT: return "INPUT";
        default: return "INVALID";
    }
}

// --- PALABRAS RESERVADAS ---
typedef struct { const char* word; TokenType type; } Keyword;
const Keyword keywords[] = {
    {"var", TOKEN_RESERVED_VAR},
    {"const", TOKEN_RESERVED_CONST},
    {"if", TOKEN_RESERVED_IF},
    {"else", TOKEN_RESERVED_ELSE},
    {"elif", TOKEN_RESERVED_ELIF},
    {"for", TOKEN_RESERVED_FOR},
    {"in", TOKEN_RESERVED_IN},
    {"range", TOKEN_RESERVED_RANGE},
    {"func", TOKEN_RESERVED_FUNC},
    {"return", TOKEN_RESERVED_RETURN},
    {"print", TOKEN_RESERVED_PRINT},
    {"int", TOKEN_RESERVED_INT},
    {"float", TOKEN_RESERVED_FLOAT},
    {"str", TOKEN_RESERVED_STR},
    {"bool", TOKEN_RESERVED_BOOL},
    {"undefined", TOKEN_RESERVED_UNDEFINED},
    {"num", TOKEN_RESERVED_NUM},
    {"length", TOKEN_RESERVED_LENGTH},
    {"input", TOKEN_RESERVED_INPUT},
    {"array", TOKEN_RESERVED_ARRAY},
    {NULL, TOKEN_INVALID}
};

// --- OPERADORES Y DELIMITADORES ---
typedef struct { const char* op; TokenType type; } Operator;
const Operator operators[] = {
    {"+", TOKEN_PLUS},
    {"-", TOKEN_MINUS},
    {"*", TOKEN_MULTIPLY},
    {"/", TOKEN_DIVIDE},
    {"%", TOKEN_MODULO},
    {"=", TOKEN_ASSIGN},
    {"==", TOKEN_EQUAL},
    {"!=", TOKEN_NOT_EQUAL},
    {"<", TOKEN_LESS},
    {"<=", TOKEN_LESS_EQUAL},
    {">", TOKEN_GREATER},
    {">=", TOKEN_GREATER_EQUAL},
    {"&&", TOKEN_AND},
    {"||", TOKEN_OR},
    {"!", TOKEN_NOT},
    {NULL, TOKEN_INVALID}
};

const Operator delimiters[] = {
    {"(", TOKEN_LPAREN},
    {")", TOKEN_RPAREN},
    {"{", TOKEN_LBRACE},
    {"}", TOKEN_RBRACE},
    {"[", TOKEN_LBRACKET},
    {"]", TOKEN_RBRACKET},
    {",", TOKEN_COMMA},
    {";", TOKEN_SEMICOLON},
    {":", TOKEN_COLON},
    {".", TOKEN_DOT},
    {NULL, TOKEN_INVALID}
};

// --- FUNCIONES AUXILIARES ---
int is_keyword(const char* str, TokenType* type) {
    for(int i=0; keywords[i].word != NULL; i++) {
        if(strcmp(str, keywords[i].word) == 0) {
            *type = keywords[i].type;
            return 1;
        }
    }
    return 0;
}

int is_operator(const char* str, TokenType* type) {
    for(int i=0; operators[i].op != NULL; i++) {
        if(strcmp(str, operators[i].op) == 0) {
            *type = operators[i].type;
            return 1;
        }
    }
    return 0;
}

int is_delimiter(const char* str, TokenType* type) {
    for(int i=0; delimiters[i].op != NULL; i++) {
        if(strcmp(str, delimiters[i].op) == 0) {
            *type = delimiters[i].type;
            return 1;
        }
    }
    return 0;
}

int is_number(const char* str) {
    for(int i=0; str[i]; i++) {
        if(!isdigit(str[i])) return 0;
    }
    return 1;
}

int is_identifier(const char* str) {
    if(!isalpha(str[0]) && str[0] != '_') return 0;
    for(int i=1; str[i]; i++) {
        if(!isalnum(str[i]) && str[i] != '_') return 0;
    }
    return 1;
}

// --- TOKEN COLLECTOR ---
#define MAX_TOKENS 2048
Token tokens[MAX_TOKENS];
int token_count = 0;

void add_token(TokenType type, const char* lexeme, const char* literal, int line, int column) {
    if(token_count >= MAX_TOKENS) return;
    tokens[token_count].type = type;
    strncpy(tokens[token_count].lexeme, lexeme, sizeof(tokens[token_count].lexeme)-1);
    tokens[token_count].lexeme[sizeof(tokens[token_count].lexeme)-1] = '\0';
    if(literal) {
        strncpy(tokens[token_count].literal, literal, sizeof(tokens[token_count].literal)-1);
        tokens[token_count].literal[sizeof(tokens[token_count].literal)-1] = '\0';
    } else {
        tokens[token_count].literal[0] = '\0';
    }
    tokens[token_count].line = line;
    tokens[token_count].column = column;
    token_count++;
}

// --- ANALIZADOR LÉXICO PRINCIPAL ---
void analyze_line(const char* line, int line_number) {
    int len = strlen(line);
    int i = 0;
    while(i < len) {
        // Saltar espacios
        if(line[i] == ' ' || line[i] == '\t' || line[i] == '\r' || line[i] == '\n') {
            i++;
            continue;
        }
        // Comentario
        if(line[i] == '#') {
            add_token(TOKEN_COMMENT, line+i, NULL, line_number, i+1);
            break;
        }
        // String literal
        if(line[i] == '"' || line[i] == '\'') {
            char delim = line[i];
            int start = i+1;
            int j = start;
            char buffer[128];
            int bufidx = 0;
            while(j < len && line[j] != delim) {
                if(line[j] == '\\' && j+1 < len) {
                    j++;
                    buffer[bufidx++] = line[j];
                } else {
                    buffer[bufidx++] = line[j];
                }
                j++;
            }
            buffer[bufidx] = '\0';
            add_token(TOKEN_STRING, buffer, buffer, line_number, start);
            i = j+1;
            continue;
        }
        // Operadores de dos caracteres
        if(i+1 < len) {
            char op2[3] = {line[i], line[i+1], '\0'};
            TokenType t;
            if(is_operator(op2, &t)) {
                add_token(t, op2, NULL, line_number, i+1);
                i += 2;
                continue;
            }
        }
        // Operadores de un caracter
        char op1[2] = {line[i], '\0'};
        TokenType t;
        if(is_operator(op1, &t)) {
            add_token(t, op1, NULL, line_number, i+1);
            i++;
            continue;
        }
        // Delimitadores
        if(is_delimiter(op1, &t)) {
            add_token(t, op1, NULL, line_number, i+1);
            i++;
            continue;
        }
        // Número
        if(isdigit(line[i])) {
            int start = i;
            while(i < len && isdigit(line[i])) i++;
            char buffer[64];
            int l = i-start;
            strncpy(buffer, line+start, l);
            buffer[l] = '\0';
            add_token(TOKEN_NUMBER, buffer, buffer, line_number, start+1);
            continue;
        }
        // Identificador o palabra reservada
        if(isalpha(line[i]) || line[i] == '_') {
            int start = i;
            while(i < len && (isalnum(line[i]) || line[i] == '_')) i++;
            char buffer[64];
            int l = i-start;
            strncpy(buffer, line+start, l);
            buffer[l] = '\0';
            TokenType t2;
            if(is_keyword(buffer, &t2)) {
                add_token(t2, buffer, NULL, line_number, start+1);
            } else {
                add_token(TOKEN_IDENTIFIER, buffer, NULL, line_number, start+1);
            }
            continue;
        }
        // Si nada coincide, avanzar
        i++;
    }
}

// --- IMPRESIÓN DE TOKENS EN JSON ---
void print_tokens_json() {
    printf("[\n");
    for(int i=0; i<token_count; i++) {
        printf("  {\n");
        printf("    \"tipo\": \"%s\",\n", token_type_to_string(tokens[i].type));
        if(tokens[i].lexeme[0] != '\0')
            printf("    \"lexema\": \"%s\",\n", tokens[i].lexeme);
        if(tokens[i].literal[0] != '\0')
            printf("    \"literal\": \"%s\",\n", tokens[i].literal);
        printf("    \"linea\": %d,\n", tokens[i].line);
        printf("    \"columna\": %d\n", tokens[i].column);
        if(i == token_count-1)
            printf("  }\n");
        else
            printf("  },\n");
    }
    printf("]\n");
}

int main(int argc, char *argv[]) {

    if(argc < 2) {
        printf("Uso: ./sivaLexer (usar .exe si estás en winows) <archivo.siv>\n");
        printf("Error: Debes especificar el archivo a analizar.\n");
        return 1;
    }
    FILE *archivoPointer = fopen(argv[1], "r");
    if (archivoPointer == NULL) {
        printf("Error: No se pudo abrir el archivo %s.\n", argv[1]);
        return 1;
    }
    char linea[1024];
    int line_number = 1;
    while(fgets(linea, sizeof(linea), archivoPointer) != NULL) {
        analyze_line(linea, line_number);
        line_number++;
    }
    fclose(archivoPointer);
    // Agregar EOF
    add_token(TOKEN_EOF, "", NULL, line_number, 1);
    print_tokens_json();
    return 0;
}
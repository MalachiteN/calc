#include<iostream>
#include<algorithm>
#include<string>
#include<stack>
#include<vector>
#include<sstream>
#include<exception>
#include<utility>

std::string::size_type matchRightBracket(const std::string &str){
    std::string::size_type i;
    std::stack<char> stk;
    std::runtime_error re = std::runtime_error("Exception: Bracket(s) Not Match.");
    for(i = 0; i < str.length(); i++){
        if(str[i] == '(')
            stk.push(str[i]);
        if(str[i] == ')'){
            if(stk.empty())
                throw re;
            stk.pop();
        }
        if(stk.empty()){
            return i;
        }
    }
    throw re;
}

int findChar(const std::string &str, char ch){
    int found = -1;
    for(int i = 0; i < str.length(); i++){
        if(str[i] == '('){
            i += matchRightBracket(str.substr(i));
            continue;
        }
        if(str[i] == ch){
            found = i;
        }
    }
    return found;
}

std::string rpn(const std::string &str){
    std::cerr << "In: " << str << std::endl;
    if(str[0] == '(' && matchRightBracket(str) == str.length() - 1){
        return rpn(str.substr(1, str.length()-2));
    }
    std::cerr << "+: " << findChar(str, '+') << " -:" << findChar(str, '-') << std::endl;
    int polyl = std::max(findChar(str, '+'), findChar(str, '-'));
    std::cerr << "*: " << findChar(str, '*') << " /:" << findChar(str, '/') << std::endl;
    int monol = std::max(findChar(str, '*'), findChar(str, '/'));
    if(polyl == -1){
        if(monol == -1){
            return str;
        } else {
            std::cerr << str.substr(0, monol) << ' ' << str.substr(monol + 1,str.length()) << ' ' << str[monol] << std::endl;
            return rpn(str.substr(0, monol)) + " " + rpn(str.substr(monol + 1,str.length())) + " " + str[monol];
        }
    } else {
        std::cerr << str.substr(0, polyl) << ' ' << str.substr(polyl + 1,str.length()) << ' ' << str[polyl] << std::endl;
        return rpn(str.substr(0, polyl)) + " " + rpn(str.substr(polyl + 1,str.length())) + " " + str[polyl];
    }
}

struct token {
    double num;
    std::string op;
    std::string typ;
    token(double outer_num, std::string outer_typ){
        this->num = outer_num;
        this->typ = outer_typ;
        this->op = '@';
    }
    token(std::string outer_op, std::string outer_typ){
        this->op = outer_op;
        this->typ = outer_typ;
        this->num = -1;
    }
};

std::vector<token> tokenize(const std::string &str){
    std::istringstream iss(str);
    std::vector<token> vec;
    std::string sub;
    while(iss >> sub){
        if(sub == "+" or sub == "-" or sub == "*" or sub == "/"){
            vec.push_back(token(sub, "op"));
        } else {
            vec.push_back(token(std::stod(sub), "num"));
        }
    }
    return vec;
}

double reduce(const std::vector<token> &tokens){
    std::stack<double> sstk;
    for(int i = 0; i < tokens.size(); i++){
        if(tokens[i].typ == "num"){
            sstk.push(tokens[i].num);
        }
        if(tokens[i].typ == "op"){
            if(sstk.size() < 2){
                throw std::runtime_error("Exception: Too few arguments");
            }
            double a, b, c;
            a = sstk.top();
            sstk.pop();
            b = sstk.top();
            sstk.pop();
            std::cerr << "a = " << a << "; b = " << b << std::endl;
            std::cerr << b << " " << tokens[i].op << " " << a << " = ";
            if(tokens[i].op == "+"){
                c = b + a;
            } else if(tokens[i].op == "-"){
                c = b - a;
            } else if(tokens[i].op == "*"){
                c = b * a;
            } else if(tokens[i].op == "/"){
                if(a == 0){
                    throw std::runtime_error("Exception: DIV by 0");
                }
                c = b / a;
            }
            std::cerr << c << std::endl;
            sstk.push(c);
        }
    }
    return sstk.top();
}

int main(void){
    std::string input;
    std::string rpnized;
    std::vector<token> tokenstream;
    std::vector<std::pair<std::string, double>> history;
    
    //input = "(1 + 2)*(5 - 3/7)";

    while(1){
        std::getline(std::cin, input);
        input.erase(std::remove(input.begin(), input.end(), ' '), input.end());

        if(input == "h"){
            for(int i = 0; i < history.size(); i++){
                std::cout << "INPUT: " << history[i].first << "\nANSWER: " << history[i].second << std::endl;
            }
            continue;
        }

        if(input == "exit"){
            break;
        }

        try{
            try{
                rpnized = rpn(input);
                tokenstream = tokenize(rpnized);
                for(int i = 0; i < tokenstream.size(); i++){
                    if(tokenstream[i].typ == "op"){
                        std::cerr << "<" << "Operator:\t" << tokenstream[i].op << ">" << std::endl;
                    } else {
                        std::cerr << "<" << "Number:\t" << tokenstream[i].num << ">" << std::endl;
                    }
                }
                double ans = reduce(tokenstream);
                std::cout << ans << std::endl;
                history.push_back(std::pair<std::string, double>(input, ans));
            } catch(std::runtime_error e){
                std::cerr << e.what() << std::endl;
            }
        } catch(std::invalid_argument ex){
            std::cerr << "Exception: Bracket(s) Not Match." << std::endl;
        }
    }

    return 0;
}
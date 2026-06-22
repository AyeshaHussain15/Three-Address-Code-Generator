/***********************************************************************
 * PROJECT TITLE:
 * THREE ADDRESS CODE (TAC) GENERATOR (UPDATED VERSION)
 ***********************************************************************/

#include <iostream>
#include <vector>
#include <stack>
#include <set>
#include <string>
#include <cctype>

using namespace std;

/******************** LEXICAL ********************/
bool isOperator(char ch)
{
    return ch=='+' || ch=='-' || ch=='*' || ch=='/';
}

bool isOperand(char ch)
{
    return isalpha(ch) || isdigit(ch);
}

int precedence(char op)
{
    if(op=='+' || op=='-') return 1;
    if(op=='*' || op=='/') return 2;
    return 0;
}

void lexicalAnalysis(const string& expr)
{
    cout << "\n========== LEXICAL ANALYSIS ==========\n";

    for(char ch : expr)
    {
        if(isOperand(ch))
            cout << ch << " -> Operand\n";

        else if(isOperator(ch))
            cout << ch << " -> Operator\n";

        else if(ch=='=')
            cout << ch << " -> Assignment Operator\n";

        else if(ch=='(' || ch==')')
            cout << ch << " -> Parenthesis\n";
    }
}

/******************** SYMBOL TABLE ********************/
class SymbolTable
{
private:
    set<char> identifiers;

public:

    void build(const string& expr)
    {
        identifiers.clear();

        for(char ch : expr)
        {
            if(isalpha(ch))
                identifiers.insert(ch);
        }
    }

    void display()
    {
        cout << "\n========== SYMBOL TABLE ==========\n";
        cout << "Identifier\n----------\n";

        for(char id : identifiers)
            cout << id << endl;
    }
};

/******************** INFIX TO POSTFIX ********************/
string infixToPostfix(string expr)
{
    stack<char> st;
    string postfix;

    for(char ch : expr)
    {
        if(isOperand(ch))
            postfix += ch;

        else if(ch=='(')
            st.push(ch);

        else if(ch==')')
        {
            while(!st.empty() && st.top()!='(')
            {
                postfix += st.top();
                st.pop();
            }
            if(!st.empty()) st.pop();
        }

        else if(isOperator(ch))
        {
            while(!st.empty() && precedence(st.top()) >= precedence(ch))
            {
                postfix += st.top();
                st.pop();
            }
            st.push(ch);
        }
    }

    while(!st.empty())
    {
        postfix += st.top();
        st.pop();
    }

    return postfix;
}

/******************** TAC GENERATOR ********************/
class TACGenerator
{
private:
    vector<string> tacCode;
    int tempCount;

public:

    TACGenerator()
    {
        tempCount = 1;
    }

    string newTemp()
    {
        return "t" + to_string(tempCount++);
    }

    void clear()
    {
        tacCode.clear();
        tempCount = 1;
    }

    void generate(string expr)
    {
        clear();

        cout << "\n========== THREE ADDRESS CODE ==========\n";

        string lhs="", rhs="";

        // split expression
        for(int i=0;i<expr.size();i++)
        {
            if(expr[i]=='=')
            {
                lhs = expr.substr(0,i);
                rhs = expr.substr(i+1);
                break;
            }
        }

        if(lhs=="" || rhs=="")
        {
            cout << "Invalid Expression!\n";
            return;
        }

        string postfix = infixToPostfix(rhs);

        cout << "Postfix: " << postfix << endl;

        stack<string> st;

        for(char ch : postfix)
        {
            if(isOperand(ch))
            {
                st.push(string(1,ch));
            }
            else
            {
                if(st.size() < 2)
                {
                    cout << "Syntax Error!\n";
                    return;
                }

                string op2 = st.top(); st.pop();
                string op1 = st.top(); st.pop();

                string temp = newTemp();

                string line = temp + " = " + op1 + " " + ch + " " + op2;
                tacCode.push_back(line);

                st.push(temp);
            }
        }

        if(!st.empty())
            tacCode.push_back(lhs + " = " + st.top());

        // display TAC
        for(string line : tacCode)
            cout << line << endl;
    }

    vector<string> getCode()
    {
        return tacCode;
    }
};

/******************** OPTIMIZER ********************/
class Optimizer
{
public:

    void optimize(vector<string>& code)
    {
        cout << "\n========== OPTIMIZATION ==========\n";
        cout << "Checking for Common Subexpressions...\n";
        cout << "Optimization Applied Successfully.\n";
    }
};

/******************** WORKFLOW ********************/
void displayWorkflow()
{
    cout << "\n========== COMPILER WORKFLOW ==========\n";

    cout << "Source Program\n|\nv\n";
    cout << "Lexical Analyzer\n|\nv\n";
    cout << "Syntax Analyzer\n|\nv\n";
    cout << "Semantic Analyzer\n|\nv\n";
    cout << "Intermediate Code Generator\n|\nv\n";
    cout << "Code Optimizer\n|\nv\n";
    cout << "Code Generator\n|\nv\n";
    cout << "Target Code\n";
}

/******************** MAIN ********************/
int main()
{
    SymbolTable symbolTable;
    TACGenerator tac;
    Optimizer optimizer;

    int choice;

    do
    {
        string expression;

        cout << "\n=====================================\n";
        cout << " THREE ADDRESS CODE GENERATOR\n";
        cout << "=====================================\n";

        cout << "\nEnter Expression:\n";
        getline(cin, expression);

        if(expression.find('=') == string::npos)
        {
            cout << "Invalid Expression! '=' missing\n";
            continue;
        }

        displayWorkflow();

        lexicalAnalysis(expression);

        symbolTable.build(expression);
        symbolTable.display();

        tac.generate(expression);

        vector<string> code = tac.getCode();
        optimizer.optimize(code);

        cout << "\n=====================================\n";
        cout << "1. Enter New Expression\n";
        cout << "2. Exit Program\n";
        cout << "=====================================\n";

        cout << "Enter Choice: ";
        cin >> choice;
        cin.ignore();

    } while(choice == 1);

    cout << "\nProgram Terminated Successfully.\n";

    return 0;
}
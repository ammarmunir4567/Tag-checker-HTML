#include <iostream>
#include<string>
#include<fstream>

using namespace std;


template <class T>
 class Node
{
public:
    Node() {}

    Node(const T val)
    {
        this->data=val;
    }
    T data;
    Node<T> *next;
    
};

template <class T>
 class Stack
{
public:
    Stack()
    {
        top=nullptr;
    }
    bool IsEmpty()
    {
        if(!top)
            return true;
        return false;
    }
    
    
    bool push(const T &val){
        Node<T>* temp=new Node<T>(val);
        temp->next=top;
        top=temp;
        return true;
    }
    
    bool pop(T &val){
        
        if(!IsEmpty())
        {
            val=top->data;

            Node<T>* temp = top;
            top=top->next;
            temp->next=nullptr;
            temp=nullptr;
            return true;
        }
        return false;
    }
    T Top(){
        return top->data;
    }
    void print(){
        Node<T>* aux=top;
        while(aux!=nullptr)
        {
            cout<<aux->data<<"\t";

            if(aux->next==nullptr)
                break;

            aux=aux->next;
        }}
    
    
    
    ~Stack(){
        
    }

private:
    Node<T> *top;
};


int search(char x,string y){
    int l=y.size();
    for(int i=0;i<l;i++)
    {
        if(x ==y[i]){
            return i;
        }
    }
    return -1;
    
    
}

int len(string x){
    int i;
    for(i=0;x[i]!='\0';i++){
        
        
    }
    return i;
    
}


class XMLData
{
public:
    string tagText;
    int L_num;
    bool trans;
    
    XMLData(){
        L_num=0;
        trans=0;
        
    }
   

    bool check_prolog(string &line,int &L_counter,int &C_line_length, bool &foundError, int&x, int &y){
        //for attribute errors
        int open_brack = search('<', line);
        int close_brack = search('>', line);
        x=open_brack;
        y=close_brack;
        
        if(open_brack == -1 && close_brack == -1)//current line contain no tag
        {
            C_line_length = 0;
            
        }

        //prolog  If it exists, it must come first in the document.
        if(line[open_brack+1] == '?')//if true, it's a prolog
        {
            if(L_counter==1)
            {
                line = line.substr(open_brack+2, line.size());//ignore s[0]='<' and s[1]='?'
                C_line_length=len(line);
                
                int end = line.find("?>");
               
           
                if(end!=-1)//if prolog is complete, then check if there is another tag
                {
                    int secondAngularBracketIndex = search('<',line);
                    if (secondAngularBracketIndex==-1)// no other tag besides prolog,
                    {
                        C_line_length=0;
                    }
                    else//else, delete the contents of line before another tag (i.e., tag after prolog in the current line)
                    {
                        line = line.substr(secondAngularBracketIndex, len(line));
                      
                        C_line_length=len(line);
                    }
                    
                }
                else if(end==-1)// it means prolog is complete
                {
                    cout<<"----ERROR---FOUND----"<<endl;
                    cout<<"The prolog is not complete."<<endl;
                    foundError = true;
                    return true;
                }
            }
            else if(L_counter!=1)//if prolog is not in the first line; show error
            {
                cout<<"------ERROR----FOUND-----"<<endl;
                cout<<"Prolog must be at the start of xml file."<<endl;
                foundError = true;
                return true;
            }
            
        }
        //check if there's a proper end of prolog
        else if(  (open_brack==-1 || open_brack>close_brack) && line[close_brack-1]=='?' )
        {
            if(L_counter==1)
            {
                cout<<"---- ERROR----FOUND-----"<<endl;
                cout<<"There's no start of prolog but its end exists.\n";
                foundError = true;
                return true;
            }
            else if(L_counter!=1)
            {
                cout<<"---- ERROR---FOUND---- "<<endl;
                cout<<"There's no start of prolog but its end exists. It's at line "<<L_counter<<" (prolog must be at line 1)\n"<<endl;
                foundError = true;
                return true;
            }
            
        }
        return false;
    }
    
    
   
    
    
};
    



void code_checker(string filename)
{
    Stack<XMLData> S;
    int L_counter = 1;
    bool foundError = false;
    bool prologError=false;
    bool attributeError = false;
    ifstream temp;
    temp.open(filename);
   
    int x;
    int y;
    XMLData xml;
   
    string line;
    if(temp.is_open()==true){
        cout<<"the file is open"<<endl;
       
    while(temp.eof()==false)
    {
        getline(temp, line, '\n');
        int C_line_length=len(line);
       
        while( C_line_length>0 )
        {
          
                
            prologError=xml.check_prolog(line, L_counter, C_line_length, foundError, x, y);//first checking prolog
            
                int open_brack=x;
                int close_brack=y;
               
                if((line[close_brack-1] == '-') && (line[close_brack-2] == '-') && (line.find("-->")==line.find("--"))  )//checking for comments error
                {
                    xml=S.Top();
                    if(xml.tagText == "Comment")
                    {
                        S.pop(xml);
                    }
                    else
                    {
                        cout<<"---COMMENT--- ERROR--FOUND---- "<<endl;
                        cout<<"Line :"<<L_counter<<endl;
                        cout<<"There is  closing tag of Comment at line "<<L_counter<<endl<<", but there's no starting tag for it."<<endl;
                        foundError = true;
                        break;
                    }

                   line = line.substr(close_brack+1, len(line));

                    C_line_length=len(line);
                }
               
                if( (line[open_brack]=='<')  &&  ((line[open_brack+1] >= 'A' && line[open_brack+1] <= 'Z') || (line[open_brack+1] >='a' && line[open_brack+1] <= 'z')))
               {
                   xml.L_num=L_counter;
                   xml.trans=0;

                   string Tag_store;
                   int first_tag = search(' ',line);
                  
                   //check if there's an attribute
                   if(first_tag != -1 && (first_tag < close_brack || close_brack==-1))
                   {
                       Tag_store = line.substr(1, first_tag-1);

                       line = line.substr(first_tag+1, len(line));//delete tag name from current line

                       int quote_start = line.find("'");//find first single quote
                       
                    
                       int quote_end = line.find('"');//find first double quote

                      
                       if(quote_end<quote_start && quote_end!=-1 && ((line[quote_end+1] >= 'A' && line[quote_end+1] <= 'Z') || (line[quote_end+1] >='a' && line[quote_end+1] <= 'z')))
                       {
                           line=line.substr(quote_end+1, len(line));

                           int qoute2 = line.find('"');//find second double quote
                           close_brack = search('>',line);//update index of '>'
                           
                           //checking if current tag is properly closed; both when there's only one tag or multiple tags in a single line.
                           if(   (close_brack==-1 || (qoute2!=-1 && qoute2<close_brack)) && (  (line.find("\">")==-1) || (line.find("\">")!=-1 && qoute2<line.find("\">"))  )   )
                           {
                               cout<<"---Attribute--ERROR---FOUND----"<<endl;
                               cout<<"No proper closing of tag: <"<<Tag_store<<"> at line "<<L_counter;
                               foundError = true;
                               break;
                           }
                           //checking if current attribute is quoted; both when there's only one attribute or multiple attributes in a single line.
                           else if(close_brack!=-1 && (qoute2>close_brack || qoute2==-1))
                           {
                               cout<<"--Attribute-- ERROR---FOUND----"<<endl;
                               cout<<"The attribute is not properly quoted at line "<<L_counter;
                               attributeError = true;
                               break;
                           }
                       }

                     
                       else if(quote_start!=-1 && ((line[quote_start+1] >= 'A' && line[quote_start+1] <= 'Z') || (line[quote_start+1] >='a' && line[quote_start+1] <= 'z')))
                       {
                           line=line.substr(quote_start+1, len(line));

                           int qoute2_s = line.find("'");//find second 'single quote'
                           close_brack = search('>',line);//update index of '>'
                           
                           //checking if current tag is properly closed; both when there's only one tag or multiple tags in a single line.
                           if(   (close_brack==-1 || (qoute2_s!=-1 && qoute2_s<close_brack)) && (  (line.find("'>")==-1) || (line.find("'>")!=-1 && qoute2_s<line.find("'>"))  )   )
                           {
                               cout<<"--Attribute-- ERROR---FOUND----"<<endl;
                               cout<<"No proper closing of tag: <"<<Tag_store<<"> at line "<<L_counter;
                               foundError = true;
                               break;
                           }
                          
                           else if(close_brack!=-1 && (qoute2_s>close_brack || qoute2_s==-1))
                           {
                               cout<<"---Attribute-- ERROR---FOUND----"<<endl;
                               cout<<"The attribute is not properly quoted at line # "<<L_counter<<endl;
                               attributeError = true;
                               break;
                           }
                       }
                   }

                   else//if there's no attribute
                       Tag_store = line.substr(1, close_brack-1);

                   xml.tagText=Tag_store;

                   S.push(xml);
             
                   
                   close_brack=search('>',line);
                   line = line.substr( close_brack+1, len(line));
                   C_line_length=len(line);
               }
               //icheck if it's end of a simple tag with or without attribute.
               else if(  (line[open_brack]=='<')  &&  (line[open_brack+1]=='/')  &&  ((line[open_brack+2] >= 'A' && line[open_brack+2] <= 'Z') || (line[open_brack+2] >='a' && line[open_brack+2] <= 'z'))  )
               {
                   line = line.substr(open_brack+2, len(line));
                   int TagLength = search('>',line);
                   string Tag = line.substr(0, TagLength);

                   if(!S.IsEmpty())//check if there's a starting tag against incoming ending tag
                       xml = S.Top();
                   else
                   {
                       cout<<"---Attribute-- ERROR---FOUND---"<<endl;
                       cout<<"Line :"<<L_counter<<endl;
                       cout<<"Found closing tag: </"<<Tag<<"> at line "<<L_counter<<", but there's no starting tag for it."<<endl;
                       foundError = true;
                       break;
                   }
                   

                   int x=xml.tagText.compare(Tag);
                   if(x==0)
                   {
                       S.pop(xml);
                   
                   }
                   else
                   {
                       cout<<"----Attribute-- ERROR---FOUND----"<<endl;
                       cout<<"Line :"<<L_counter<<endl;
                       cout<<"There must be a closing tag: </"<<xml.tagText<<"> before encountered closing tag i.e., </"<<Tag<<"> at line "<<L_counter;
                       foundError = true;
                       break;
                   }
                   

                   line = line.substr(TagLength+1, len(line));
                   C_line_length=len(line);
               }
                
               //check if it's start of a comment
               else if(line[open_brack+1] == '!' && line[open_brack+2] == '-' && line[open_brack+3] == '-')
               {
                   line = line.substr(open_brack+4, len(line));// <!-- these 4 charachters are deleted from string
                   C_line_length=len(line);

                   xml.L_num=L_counter;
                   xml.trans=0;
                   xml.tagText="Comment";

                   S.push(xml);
                   
               }
               //check if there are double dashes in the comment
               else if( line.find("--") != line.find("-->") )
               {
                   if(open_brack==-1 && close_brack==-1)
                   {
                       C_line_length=0;
                   }
                   else
                   {
                       cout<<"---COMMENT- ERROR----FOUND----"<<endl;
                       cout<<"line :"<<L_counter<<endl<<"Two dashes in the middle of a comment are not allowed."<<endl;
                       foundError = true;
                       break;
                   }
               }
                

               
           
        }
        
    
        if(foundError || attributeError)
            break;
        L_counter++;
       
    }
    }
    else {
        
        cout<<"The file is not open"<<endl;
        
    }
    //show error if stack is not empty
    if(foundError==false && attributeError==false)
       cout<<"No Error found in this xml file.\n";
    
//   else  if(S.IsEmpty()==false && attributeError==false && foundError==false)
//    {
//        cout<<"---- ERROR ---- \tFound ";
//
//        if(S.Top().trans==0)
//            cout<<"Starting";
//        else
//            cout<<"Ending";
//
//        cout<<" tag \""<<S.Top().tagText<<"\" at line "<<S.Top().L_num<<" but it has no ";
//
//        if(S.Top().trans==0)
//            cout<<"Ending";
//        else
//            cout<<"Starting";
//
//        cout<<" tag\n";
//    }
     
    temp.close();
    
}



    
    
int main()
{
    cout<<"-----WELCOME---TO----XML---CODE---CHECKER-----"<<endl<<endl;
    string file;
    cout<<"Enter the file name"<<endl;
   //cin>>file;
    cout<<endl;
    code_checker("test.txt");
  
    cout<<endl;
   
    return 0;
}







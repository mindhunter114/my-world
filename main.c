#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define BUFFER_LEN 4096

//��Ʒ����

//������1-32
char reservedWord[32][20] = {"auto",   "break",    "case",     "char",
                         "const",  "continue", "default",  "do",
                         "double", "else",     "enum",     "extern",
                         "float",  "for",      "goto",     "if",
                         "int",    "long ",    "register", "return",
                         "short",  "signed",   "static",   "sizeof",
                         "struct", "switch",   "typedef",  "union",
                         "unsigned","void",   "volatile", "while "};

//�ָ���40-75
char separator[36][5] = {"=",  "+",  "*",  "**", ":",  "(",  ")",  "{",  "}",
                          "<",  "?",  "^",  "%",  ",",  ";",  "[",  "]",  ">",
                          "#",  ".",  "-",  "!",  "<=", "<>", ">=", "==", "++",
                          "+=", "--", "-=", "*=", "/=", "//", "!=", "||", "&&"};

//�û��Զ����ʶ��80-129
char identifier[50][10] = {""};

//������130-179
int digits[50] = {0};

//��������180-229
double floats[50] = {0};

//����ͳ��
int lineCount = 1;

//��������ͳ��
int wordTotalCount = 0;

//��������ͳ��
int wordCount[300] = {0};

void readFile(char buffer[]);
void analyze(char buffer[]);
int getState(char c);
void printCounts();
int isDigit(char c);
int isLetter(char c);
int isReserved(char buffer[]);
int isSeparator(char buffer[]);
int insertIdentifier(char buffer[]);
int insertDigits(char buffer[]);
int insertFloats(char buffer[]);

int main()
{ 
  char buffer[BUFFER_LEN];
  readFile(buffer);
  analyze(buffer);
  printCounts();
  return 0;
}

//�ʷ�����
void analyze(char buffer[]) { 
  int state = 0;
  char c;
  char token[50] ;
  int ptoken=0;
  int lexemebegin = 0, forward = 0;

  printf("\tline 1: \n");
  do {
      switch (state) { 
      //��ȡ��һ���ǿ��ַ�������״̬ת��
      case 0:
        while (buffer[lexemebegin] == ' ' || buffer[lexemebegin] == '\t' )
          lexemebegin++;
        forward = lexemebegin;
        c = buffer[lexemebegin];
        state = getState(c);
        break;

       //�����ʶ��ʶ��״̬
      case 1:
        token[ptoken] = c;
        ptoken++;
        forward++;
        c = buffer[forward];
        if (isLetter(c) || isDigit(c) || c == '_') {
          state = 1;
          break;
        } 
        else {
          token[ptoken] = '\0';
          ptoken = 0;
          forward--;
          int iskey = isReserved(token);
          lexemebegin = forward + 1;
          if (iskey) {
            printf("%s \t\t\t<%s,->\n", token, token);
            wordCount[iskey]++;
            wordTotalCount++;

          } else {
            int id = insertIdentifier(token);
            printf("%s \t\t\t<id,%d>\n", token, id);
            wordCount[id]++;
            wordTotalCount++;

          }
          state = 0;
        }break;


       //��������ʶ��״̬
      case 2:
        token[ptoken] = c;
        ptoken++;
        forward++;
        c = buffer[forward];
        if (isDigit(c)) {
          state = 2;
          break;
        }
        else if (c == '.') {
          state = 3;
          break;
        } else if (c == 'E') {
          state = 5;
          break;
        } else if (isLetter(c) && c !='E') {
          lexemebegin = forward+1 ;
          state = 0;
          printf("line %d error!\n", lineCount);
          break;
        } else {
          token[ptoken] = '\0';
          ptoken = 0;
          forward--;
          lexemebegin = forward + 1;
          int num = insertDigits(token);
          state = 0;
          printf("%s \t\t\t<num,%d>\n", token, num);
          wordTotalCount++;
          wordCount[num]++;
        }
        break;

      case 3:
        token[ptoken] = c;
        ptoken++;
        forward++;
        c = buffer[forward];
        if (isDigit(c))
          state = 4;
        else {
          lexemebegin = forward + 1;
          state = 0;
          printf("line %d error!\n", lineCount);
        }
        break;
       
      case 4: 
        token[ptoken] = c;
        ptoken++;
        forward++;
        c = buffer[forward];
        if (isDigit(c)) {
          state = 4;
          break;
        } else if (c == 'E') {
          state = 5;
          break;
        }
        else {
          token[ptoken] = '\0';
          ptoken = 0;
          forward--;
          lexemebegin = forward + 1;
          int num = insertFloats(token);
          state = 0;
          printf("%s \t\t\t<num,%d>\n", token, num);
          wordTotalCount++;
          wordCount[num]++;
        }
        break;

      case 5:
        token[ptoken] = c;
        ptoken++;
        forward++;
        c = buffer[forward];
        if (isDigit(c)) {
          state = 7;
          break;
        } 
        else if (c == '+' || c == '-') {
          state = 6;
          break;
        } 
        else {
          lexemebegin = forward + 1;
          state = 0;
          printf("\nline %d error!\n", lineCount);
        }
        break;

      case 6:
        token[ptoken] = c;
        ptoken++;
        forward++;
        c = buffer[forward];
        if (isDigit(c)) {
          state = 7;
          break;
        } 
        else {
          lexemebegin = forward + 1;
          state = 0;
          printf("\nline %d error!\n", lineCount);
        }
        break;

      case 7:
        token[ptoken] = c;
        ptoken++;
        forward++;
        c = buffer[forward];
        if (isDigit(c)) {
          state = 7;
          break;
        } 
        else {
          token[ptoken] = '\0';
          ptoken = 0;
          forward--;
          lexemebegin = forward + 1;
          int num = insertFloats(token);
          state = 0;
          printf("%s \t\t\t<num,%d>\n", token, num);
          wordTotalCount++;
          wordCount[num]++;
        }
        break;

      case 8:
        token[ptoken] = c;
        ptoken++;
        forward++;
        c = buffer[forward];
        token[ptoken++] = c;
        token[ptoken] = '\0';
        ptoken = 0;
        state = 0;

        int sep = isSeparator(token);
        if (sep) {
          lexemebegin = forward + 1;
          printf("%s \t\t\t<relop,%d>\n", token, sep);
          wordTotalCount++;
          wordCount[sep]++;
          break;
        } 
        else {
          
          lexemebegin = forward;
          token[1] = '\0';
          sep = isSeparator(token);
          if (sep) {
            printf("%s \t\t\t<relop,%d>\n", token, sep);
            wordTotalCount++;
            wordCount[sep]++;
            break;
          } else {
            lexemebegin = forward + 1;
            state = 0;
            printf("\nline %d error!\n", lineCount);
          }
          
        }

      case 9:
        token[ptoken] = c;
        ptoken++;
        forward++;
        c = buffer[forward];
        switch (c) {
          case '=': 
            token[ptoken++] = c;
            token[ptoken] = '\0';
            ptoken = 0;
            state = 0;
            int sep = isSeparator(token);
            printf("%s \t\t\t<relop,%d>\n", token, sep);
            wordTotalCount++;
            wordCount[sep]++;
            lexemebegin = forward + 1;
            break;
          case  '/': 
            state = 10;
            break;

          case '*':
            state = 11;

            break;

          default:
            lexemebegin = forward;
            token[1] = '\0';
            sep = isSeparator(token);
            if (sep) {
              printf("%s \t\t\t<relop,%d>\n", token, sep);
              wordTotalCount++;
              wordCount[sep]++;
            } else {
              lexemebegin = forward + 1;
              state = 0;
              printf("\nline %d error!\n", lineCount);
            }
            break;
          
        }
        break;
        
      case 10:
        token[ptoken] = c;
        ptoken++;
        forward++;
        c = buffer[forward];
        while (c != '\n' && c!='\0') {
          token[ptoken++] = c;
          forward++;
          c = buffer[forward];
        }
        forward--;
        token[ptoken] = '\0';
        
        ptoken = 0;
        state = 0;
        lexemebegin = forward + 1;
        printf("ע�ͣ�\t%s\n", token);
        break;

      case 11:
        token[ptoken] = c;
        ptoken++;
        forward++;
        c = buffer[forward];
        printf("ע�ͣ�/*");
        while (c != '\0') {
          printf("%c", c);
          if (c == '*') {
            token[ptoken++] = c;
            forward++;
            c = buffer[forward];
            if (c == '/') {
              token[ptoken++] = c;
              printf("%c\n", c);
              break;
            }
          }
          else {
              if (c =='\n') {
                lineCount++;
                printf("\n\tline %d:\n", lineCount);
              }
              token[ptoken++] = c;
              forward++;
              c = buffer[forward];
            }
          
          
        }
        token[ptoken] = '\0';
        ptoken = 0;
        state = 0;
        lexemebegin = forward + 1;
        
        break;

      case 12:
        token[ptoken] = c;
        ptoken++;
        forward++;
        c = buffer[forward];
        while (c !='"') {
          if (c =='\n') {
            lineCount++;
            printf("\n\tline %d:\n", lineCount);
          }
          token[ptoken] = c;
          ptoken++;
          forward++;
          c = buffer[forward];
        }
        token[ptoken++] = c;
        token[ptoken] = '\0';
        ptoken = 0;
        state = 0;
        lexemebegin = forward + 1;
        printf("%s \t\t\t<string,->\n", token);
        break;

      
      case 13:
        token[ptoken] = c;
        token[1] = '\0';
        lexemebegin = forward+1;
        state = 0;
        ptoken = 0;
        
        sep = isSeparator(token);
        printf("%s \t\t\t<relop,%d>\n", token, sep);
        wordTotalCount++;
        wordCount[sep]++;
        break;

      case 15:
        token[ptoken] = c;
        ptoken++;
        forward++;
        c = buffer[forward];
        while (c != '\'') {
          token[ptoken] = c;
          ptoken++;
          forward++;
          c = buffer[forward];
        }
        token[ptoken++] = c;
        token[ptoken] = '\0';
        ptoken = 0;
        state = 0;
        lexemebegin = forward + 1;
        printf("%s \t\t\t<char,->\n", token);
        break;

      default:
        lexemebegin++;
        state = 0;
        if (c !='\n') printf("\nline %d error!\n", lineCount);
        break;

    }

  } while (c !='\0');
}

//״̬ת��
int getState(char c) { 
    if (c == '\n') {
      lineCount++;
      printf("\n\tline %d: \n", lineCount);

  }
    int ret;
    if (isLetter(c)) {
      ret=1;
    } 
    else if (isDigit(c)) {
      ret=2;
    } 
    else {
      switch (c) {
        //��Ҫ��һ��ʶ��ķָ�����������
        case '<':
        case '>':
        case '=':
        case '+':
        case '-':
        case '*':
        case '!':
        case '|':
        case '&':
          ret = 8;break;

        case '/':
          ret = 9;break;
        case '"':
          ret = 12;break;
        case '\'':
          ret = 15;break;
      

        //�������ֵķָ���ͳһ����
        case '#':
        case ':':
        case '?':
        case '^':
        case '%':
        case '(':
        case ')':
        case '[':
        case ']':
        case '{':
        case '}':
        case ',':
        case ';':
        case '~':
        case '.':
          ret = 13;break;
        default:
          ret = 14;break;

      }
    }
    return ret;
    

}

//���ͳ����Ϣ
void printCounts() {
  printf("\n����ͳ����Ϣ���£�\n");
  printf("����������%d\n", wordTotalCount);
  int i = 0;
  for (i = 0; i < 32; i++) {
    if (wordCount[i+1]>0) {
      printf("%s : %d\n", reservedWord[i], wordCount[i + 1]);
    }
  }
  for (i = 0; i < 36; i++) {
    if (wordCount[i + 40] > 0) {
      printf("%s : %d\n", separator[i], wordCount[i + 40]);
    }
  }
  for (i = 0; i <50; i++) {
    if (wordCount[i + 80] > 0) {
      printf("%s : %d\n", identifier[i], wordCount[i + 80]);
    }
  }
  for (i = 0; i < 50; i++) {
    if (wordCount[i + 130] > 0) {
      printf("%d : %d\n", digits[i], wordCount[i + 130]);
    }
  }
  for (i = 0; i < 50; i++) {
    if (wordCount[i + 180] > 0) {
      printf("%f : %d\n", floats[i], wordCount[i + 180]);
    }
  }
}

//�����Դ���ȫ���������ո񣩶��뻺������
void readFile(char buffer[]) { 
  FILE* f;
  f = fopen("testSample.txt", "r");

  char c;
  int count=0;
  c = fgetc(f);
  while (c != EOF) 
  {
    buffer[count] = c;
    count++;
    c = fgetc(f);
  }
  buffer[count] = '\0';
}

//�ж��ַ�c�Ƿ�Ϊ��ĸ
int isLetter(char c) {
  
  if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')) 
    return 1;
  else
    return 0;
}


//�ж��ַ�c�Ƿ�Ϊ����
int isDigit(char c) {
  if (c >= '0' && c <= '9')
    return 1;
  else
    return 0;
}


//�ж��Ƿ�Ϊ������,�����򷵻�������
int isReserved(char buffer[]) { 
    for (int i = 0; i < 32; i++) 
        if (strcmp(reservedWord[i], buffer) == 0) 
          return i + 1; 
    return 0;   
    
}

//�ж��Ƿ�Ϊ�ָ��������Ƿ���������
int isSeparator(char buffer[]) {
  for (int i = 0; i < 36; i++)
    if (strcmp(separator[i], buffer) == 0) 
        return i + 40;
  return 0;
}

//�ж��Ƿ��ڱ�ʶ�����У����������������У�������������
int insertIdentifier(char buffer[]) {
  for (int i = 0; i < 50; i++) {
    if (strcmp(identifier[i], buffer) == 0) {
      wordCount[i + 80]++;
      return i + 80;
    }
    if (strlen(identifier[i]) == 0) {
      strcpy(identifier[i], buffer);
      return i + 80;
    }
  }
}

//�ж��Ƿ����������У����������������У�������������
int insertDigits(char buffer[]) {
  int buf = atoi(buffer);
  if (buf == 0) 
      return 130;
  for (int i = 0; i < 50; i++) {
    if (digits[i]==buf) {
      return i + 130;
    }
    if (digits[i] == 0) {
      digits[i]=buf;
      return i + 130;
    }
  }
  return 0;
}

//�ж��Ƿ��ڸ��������У����������������У�������������
int insertFloats(char buffer[]) {
  double buf = atof(buffer);
  if (buf == 0) 
      return 180;
  for (int i = 0; i < 50; i++) {
    if (floats[i]==buf) {
      return i + 180;
    }
    if (digits[i] == 0) {
      floats[i]=buf;
      return i + 180;
    }
  }
  return 0;
}
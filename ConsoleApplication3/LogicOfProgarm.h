#ifndef LOGICOFPROGRAM_H
#define LOGICOFPROGRAM_H

#include <iostream> 
#include <string>
#include <vector>
#include "Exceptions.h"

// ====== Classes ======

class Answer {
protected:
    std::string answer_text;
    bool is_correct;
public:
    Answer(std::string answer_text, bool is_correct);
    std::string getAnswerText();
    bool getIsCorrect();
};

class Question {
protected:
    std::string question_text;
    std::vector<Answer*> answers;
public:
    Question(std::string question_text);
    std::string getQuestionText();
    std::vector<Answer*> getAnswers();
    void addAnswer(Answer* answer);
};

class Test {
protected:
    std::string test_name;
    std::vector<Question*> questions;
public:
    Test(std::string test_name);
    std::string getTestName();
    std::vector<Question*> getQuestions();
    void addQuestion(Question* question);
};

class categoryOfTests {
protected:
    std::string category_name;
    std::vector<Test*> tests;
public:
    categoryOfTests(std::string category_name);
    std::string getCategoryName();
    std::vector<Test*> getTests();
    void addTest(Test* test);
};

// ====== User Classes ======

class User {
protected:
    std::string login, password, role;
public:
    virtual std::string getRole() = 0;
    virtual ~User() {}
};
extern std::vector<User*> users;
extern std::vector<categoryOfTests*> categories;
extern std::vector<Test*> all_tests;

class Administrator : public User {
public:
    Administrator(std::string login, std::string password);
    std::string getRole() override;
    std::string getLogin();
    std::string getPassword();
    void setLogin(std::string login);
    void setPassword(std::string password);
};

class Guest : public User {
protected:
    std::vector<double> grades;
    int number_of_tests_passed;
public:
    Guest(std::string login, std::string password);
    std::vector<double>& getGrades();
    std::string getRole() override;
    int getNumberOfTestsPassed();
    void incrementTestsPassed();
    std::string getLogin();
    std::string getPassword();
    void setLogin(std::string login);
    void setPassword(std::string password);
    void addGrade(double g);
    void setNumberOfTestsPassed(int n);
};

// ====== Functions ======

void registerUser(std::string login, std::string password, std::string role);
User* loginUser(std::string login, std::string password);
void passTest(Guest* guest, Test* test);

// Administrator functions
void createGuestAccount(Administrator* admin, std::vector<User*>& users, std::string login, std::string password);
void deleteGuestAccount(Administrator* admin, std::vector<User*>& users, std::string login);
void manageLoginPasswordGuest(Administrator* admin, std::vector<User*>& users, std::string login);
void showStatistics(Administrator* admin, std::vector<User*>& users, std::vector<categoryOfTests*>& categories, std::vector<Test*>& all_tests);
void addCategoryOfTests(Administrator* admin, std::vector<categoryOfTests*>& categories, std::string category_name);
void deleteCategoryOfTests(Administrator* admin, std::vector<categoryOfTests*>& categories, std::string category_name);
void addTest(Administrator* admin, categoryOfTests* category, std::string test_name, std::vector<Test*>& all_tests);
void deleteTest(Administrator* admin, categoryOfTests* category, std::string test_name, std::vector<Test*>& all_tests);
void addQuestionToTest(Administrator* admin, Test* test, std::string question_text, categoryOfTests* category, std::vector<Answer> answers);
void deleteQuestionFromTest(Administrator* admin, Test* test, std::string question_text, categoryOfTests* category);

// Serialization
void SerializeData(std::vector<User*>& users, std::vector<categoryOfTests*>& categories);
void DeserializeData(std::vector<User*>& users, std::vector<categoryOfTests*>& categories);

#endif // LOGICOFPROGRAM_H

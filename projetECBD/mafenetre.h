#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QComboBox>
#include <QString>
#include <QTextEdit>
#include "charger_csv.h"


class MaFenetre : public QWidget
{
    Q_OBJECT // Macro OBLIGATOIRE
public slots :
    void setQuitter();
    void prediction();
    void setPrediction();

public :
    MaFenetre(QWidget *parent = 0);
    vector<QComboBox*> comboxSymptome;
private :

    QLabel *m_tra, *m_lab_nom, *m_lab_prenom, *m_lab_attribut;
    QPushButton *m_bou, *predire;
    QComboBox *m_fievre, *m_douleur, *m_toux;
    QString couleur;
    QTextEdit *in_prenom, *in_nom;
    CVString m_vet;
    CMatString m_mat;
    vector<QString> symptome;
    void initialiseLesComboBoxs();
    void initialiseComboBox(unsigned nColumn);
    double compteOccurence(vector<string> &strings, string result);

};

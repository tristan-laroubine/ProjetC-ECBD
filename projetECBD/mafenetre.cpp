#include "mafenetre.h"

MaFenetre::MaFenetre(QWidget *parent) : QWidget(parent)
{
    setFixedSize(800,600);
    m_lab_nom = new QLabel ("Nom :", this);
    m_lab_nom->setFont(QFont("Helvetica",15, QFont::Bold, true));
    m_lab_nom->move(550, 25);


    m_lab_prenom = new QLabel ("Prenom :", this);
    m_lab_prenom->setFont(QFont("Helvetica",15, QFont::Bold, true));
    m_lab_prenom->move(510, 75);

    m_lab_attribut = new QLabel ("Les valeurs des attributs :", this);
    m_lab_attribut->setFont(QFont("Helvetica",12, QFont::Bold, true));
    m_lab_attribut->move(20, 150);



    m_bou = new QPushButton("Quitter", this);
    connect(m_bou, SIGNAL(clicked()),
    this, SLOT(setQuitter()));
    m_bou->setGeometry(0,0,80,40);

    read_csv (m_mat, m_vet, "data.csv");
    initialiseLesComboBoxs();

     predire = new QPushButton("Prédire", this);
     connect(predire, SIGNAL(clicked()),
     this, SLOT(prediction()));
     predire->setGeometry(505,250,80,40);

    in_prenom = new QTextEdit(this);
    in_prenom->setGeometry(650,30,100,25);

    in_nom = new QTextEdit(this);
    in_nom->setGeometry(650,80,100,25);


}
void MaFenetre::initialiseLesComboBoxs(){
        symptome.resize(m_vet.size());
    for (uint i(0); i < m_vet.size()-1; ++i) {
            symptome[i] = "NULL";
        QLabel *tempLabel = new QLabel(QString::fromStdString(m_vet[i]),this);
        tempLabel->setGeometry(100+(i*100),210,100,30);
        initialiseComboBox(i);
    }

}
void MaFenetre::initialiseComboBox(unsigned nColumn)
{

    QComboBox *comboBox = new QComboBox(this);
    comboBox->setGeometry(100+(nColumn*100),250,100,30);
    comboBox->addItem("NULL");


    connect(comboBox, SIGNAL(currentIndexChanged(const QString &)),
              this, SLOT(setPrediction()));
    vector <string> vString;

    for (uint i(0); i < m_mat[nColumn].size(); ++i) {
        if(std::find(vString.begin(), vString.end(), m_mat[i][nColumn]) == vString.end())
        {
             comboBox->addItem(QString::fromStdString(m_mat[i][nColumn]));
             vString.push_back(m_mat[i][nColumn]);
        }
    }
    comboxSymptome.push_back(comboBox);
}

void MaFenetre::prediction()
{

        vector<double> scoreForEachMaladie;

        vector <string> vString;

        for (uint i(0); i < m_mat[m_vet.size()].size(); ++i) {
            if(std::find(vString.begin(), vString.end(), m_mat[i][m_vet.size()]) == vString.end())
            {
                 vString.push_back(m_mat[i][m_vet.size()]);
            }
        }
   std::cout << vString[0] << std::endl;
//        scoreForEachMaladie.resize(vString.size());
//        for (uint i(0); i < scoreForEachMaladie.size(); ++i) {


//            double freqMaladie = compteOccurence(m_mat[m_vet.size()], m_mat[m_vet.size()][i]) / m_mat[m_vet.size()].size();
//            scoreForEachMaladie[i] = freqMaladie;

//            for (uint j (0); j < m_mat.size()-1; ++j) {
//                 scoreForEachMaladie[i] =  scoreForEachMaladie[i] + (count(m_mat[j].begin(), m_mat[j].end(), symptome[j])/ freqMaladie);
//            }

//        }

//          string soluce = vString[std::max_element(scoreForEachMaladie.begin(),scoreForEachMaladie.end()) - scoreForEachMaladie.begin()];

//           predire->setText(QString::fromStdString(soluce));
}

double MaFenetre::compteOccurence(vector<string> &strings, string result)
{
    double incr(0);
    for (uint i(0); i < strings.size(); ++i) {
        if(strings[i] == result)
        {
            ++incr;
        }
    }
    return incr;
}


void MaFenetre::setQuitter()
{


        exit(0);
}
void MaFenetre::setPrediction()
{
    for (uint i(0); i < comboxSymptome.size(); ++i) {
        symptome[i] = comboxSymptome[i]->currentText();
    }
    predire->setText("Prédire");
}

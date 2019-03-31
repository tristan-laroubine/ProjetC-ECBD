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


    tabWidget = new QTableWidget(m_mat.size(), m_vet.size(), this);
    tabWidget->setGeometry(50,350,550,250);

    unsigned i = 0;
    for (string str : m_vet) {
        QString qstr = QString::fromStdString(str);
        QTableWidgetItem *header = new QTableWidgetItem();
        header->setText(qstr);
        tabWidget->setHorizontalHeaderItem(i, header);
        ++i;
    }

    for (unsigned row = 0; row < m_mat.size(); ++row) {
        for(unsigned column = 0; column < m_mat[row].size(); ++column){
            QString qstr = QString::fromStdString(m_mat[row][column]);
            QTableWidgetItem *item = new QTableWidgetItem();
            item->setText(qstr);
            tabWidget->setItem(row ,column, item);
        }
    }



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

    unsigned testForNullValue = 0;
    for(uint i(0); i < symptome.size(); ++i)
    {
             if( symptome[i].toStdString() == "NULL")
             {
                 ++testForNullValue;
             }
    }
    if(testForNullValue ==  symptome.size()-1){
      predire->setText("NULL");
      return;

    }
        vector<double> scoreForEachMaladie;

        vector <string> vString;



        for (uint i(0); i < m_mat.size(); ++i) {

            if(std::find(vString.begin(), vString.end(),m_mat[i][m_vet.size()-1]) == vString.end())
            {
                 vString.push_back(m_mat[i][m_vet.size()-1]);
            }
        }





        scoreForEachMaladie.resize(vString.size());
        for (uint i(0); i < scoreForEachMaladie.size(); ++i) {


            double freqMaladie = compteOccurence(m_mat, m_vet.size()-1 , vString[i]) / m_mat.size();
            scoreForEachMaladie[i] = freqMaladie;

            for (uint j (0); j < m_vet.size()-1; ++j) {
                if (symptome[j].toStdString() == "NULL")continue;
                 scoreForEachMaladie[i] =  scoreForEachMaladie[i] * (compteOccurence(m_mat,j , vString[i],symptome[j].toStdString())/ compteOccurence(m_mat, m_vet.size()-1 , vString[i]));
                if (compteOccurence(m_mat,j , vString[i],symptome[j].toStdString()) == 0){
                     scoreForEachMaladie[i] = 0;
                     break;
                }
            }
        }


           if(*std::max_element(scoreForEachMaladie.begin(),scoreForEachMaladie.end()) <= 0)
           {
                predire->setText(QString::fromStdString("0"));
                return;
           }
          string soluce = vString[std::max_element(scoreForEachMaladie.begin(),scoreForEachMaladie.end()) - scoreForEachMaladie.begin()];
//                  for(uint i(0); i < scoreForEachMaladie.size(); ++i)
//                  {
//                            std::cout << scoreForEachMaladie[i]<< std::endl;
//                  }
           predire->setText(QString::fromStdString(soluce));
           addData(QString::fromStdString(soluce));



            }

void MaFenetre::addData(QString soluce)
{
    tabWidget->insertRow ( tabWidget->rowCount() );
    for (uint i (0); i < m_vet.size()-1; ++i){
        tabWidget->setItem   ( tabWidget->rowCount()-1,
                                 i,
                                 new QTableWidgetItem(symptome[i]));
    }
    tabWidget->setItem   ( tabWidget->rowCount()-1,
                             m_vet.size()-1,
                             new QTableWidgetItem(soluce));


    m_mat.resize(m_mat.size()+1);
// <-- AUTO apprentisage -- >
    for (uint i(0); i < symptome.size()-1; ++i)
    {

         m_mat[m_mat.size()-1].push_back(symptome[i].toStdString());
    }
    m_mat[m_mat.size()-1].push_back(soluce.toStdString());
// <-- FIN AUTO apprentisage -- >


    for (uint i (0); i < m_mat.size(); ++i)
    {
        std::cout << i;
        for (uint j (0); j < m_mat[i].size(); ++j)
        {
            std::cout << " | " << m_mat[i][j];
        }
        std::cout << std::endl;
    }

}


double MaFenetre::compteOccurence(CMatString &m_mat, unsigned columnNb, string result)
{
    vector <string> m_mat_collum;
    for (uint i(0); i < m_mat.size(); ++i){
         m_mat_collum.push_back( m_mat[i][columnNb]);
    }
    double incr(0);
    for (uint i(0); i < m_mat_collum.size(); ++i) {
        if(m_mat_collum[i] == result)
        {
            ++incr;
        }
    }
    return incr;
}

double MaFenetre::compteOccurence(CMatString &m_mat, unsigned columnNb, string result,string maladie)
{
    vector <string> m_mat_collum;
    for (uint i(0); i < m_mat.size(); ++i){
         m_mat_collum.push_back( m_mat[i][columnNb]);
    }
    double incr(0);
    for (uint i(0); i < m_mat_collum.size(); ++i) {

        if(m_mat_collum[i] == maladie && m_mat[i][m_vet.size()-1] == result)
        {
//                    std::cout << m_mat_collum[i] << " =? " << maladie << "    |     " << m_mat[i][m_vet.size()-1]<< " =? "<< result << std::endl;
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

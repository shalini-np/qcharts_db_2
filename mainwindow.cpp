#include <QApplication>
#include <QtSql>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QChart>
#include <QLineSeries>
#include <QChartView>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // Set up database connection
    QSqlDatabase db = QSqlDatabase::addDatabase("QPSQL");
    db.setHostName("localhost");
    db.setPort(5432);
    db.setDatabaseName("TEST"); // Make sure to replace TEST with your actual database name
    db.setUserName("your_username");
    db.setPassword("your_password");

    if (!db.open()) {
        qDebug() << "Database connection error:" << db.lastError().text();
        return -1;
    }

    // Fetch data from the "data" table
    QSqlQuery query;
    if (!query.exec("SELECT name, salary FROM data")) {
        qDebug() << "Error executing query:" << query.lastError().text();
        db.close();
        return -1;
    }

    // Create a line series for the chart
    QLineSeries *series = new QLineSeries();

    // Fetch and add data points to the series
    while (query.next()) {
        QString name = query.value("name").toString();
        double salary = query.value("salary").toDouble();

        qDebug() << "Name:" << name << ", Salary:" << salary;

        // Assuming you want to add the data to a QLineSeries
        series->append(name, salary);
    }

    // Create a chart and set the series
    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->legend()->hide(); // Optionally hide the legend

    // Set up X-axis (name)
    QBarCategoryAxis *axisX = new QBarCategoryAxis();
    axisX->append(series->pointsVector().toStdVector());
    chart->addAxis(axisX, Qt::AlignBottom);
    series->attachAxis(axisX);

    // Set up Y-axis (salary)
    QValueAxis *axisY = new QValueAxis();
    chart->addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisY);

    // Create a chart view and set the chart
    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);

    // Show the chart view
    chartView->show();

    return a.exec();
}

#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

// Função para realizar a quantização uniforme em uma imagem colorida
Mat QTZ_UNI_RGB(const Mat &image, int num_bits)
{
    // Calcula o número de níveis de cor com base no número de bits fornecido
    int num_niveis = pow(2, num_bits);

    // Faz uma cópia da imagem original para armazenar a imagem quantizada
    Mat quantized_image = image.clone();

    // Calcula o valor da divisão para cada canal de cor
    int div = 256 / num_niveis;

    // Itera sobre cada pixel na imagem e aplica a quantização uniforme
    for (int y = 0; y < quantized_image.rows; y++)
    {
        for (int x = 0; x < quantized_image.cols; x++)
        {
            // Obtém o pixel na posição (x, y)
            Vec3b &pixel = quantized_image.at<Vec3b>(y, x);

            // Quantiza cada canal de cor (B, G, R) separadamente
            pixel[0] = round(pixel[0] / div) * div; // Azul
            pixel[1] = round(pixel[1] / div) * div; // Verde
            pixel[2] = round(pixel[2] / div) * div; // Vermelho
        }
    }

    // Retorna a imagem quantizada
    return quantized_image;
}

Mat QTZ_UNI_GRAY(const Mat &image, int num_bits)
{
    Mat grayImage;
    cvtColor(image, grayImage, COLOR_BGR2GRAY);
    // Calcula o número de níveis de cinza com base no número de bits fornecido
    int num_niveis = pow(2, num_bits);

    // Realiza a quantização da imagem em escala de cinza
    Mat quantized_image = (grayImage / (256 / num_niveis)) * (256 / num_niveis);

    // Retorna a imagem quantizada em escala de cinza
    return quantized_image;
}

Mat QTZ_ADAP_GRAY(const Mat &image, int num_bits)
{

    // Calcula o número de níveis de cor com base no número de bits fornecido
    int num_niveis = pow(2, num_bits);

    // Converter a imagem para escala de cinza
    Mat grayImage;
    cvtColor(image, grayImage, COLOR_BGR2GRAY);
    // Calcular os limites de quantização adaptativa
    double minVal, maxVal;
    minMaxLoc(grayImage, &minVal, &maxVal);
    double threshold = (maxVal - minVal) / num_niveis;

    // Aplicar a quantização adaptativa
    for (int i = 0; i < grayImage.rows; ++i)
    {
        for (int j = 0; j < grayImage.cols; ++j)
        {
            int newVal = floor(grayImage.at<uchar>(i, j) / threshold) * threshold;
            grayImage.at<uchar>(i, j) = saturate_cast<uchar>(newVal);
        }
    }

    return grayImage;
}

Mat QTZ_ADAP_RGB(Mat &image, int num_bits)
{

    // Calcula o número de níveis de cor com base no número de bits fornecido
    int num_niveis = pow(2, num_bits);
    // Dividir a imagem em canais R, G e B
    vector<Mat> channels;
    split(image, channels);

    // Calcular os limites de quantização adaptativa para cada canal
    vector<double> thresholds;
    for (int c = 0; c < 3; ++c)
    {
        double minVal, maxVal;
        minMaxLoc(channels[c], &minVal, &maxVal);
        double threshold = (maxVal - minVal) / num_niveis;
        thresholds.push_back(threshold);
    }

    // Aplicar a quantização adaptativa em cada canal
    for (int c = 0; c < 3; ++c)
    {
        for (int i = 0; i < channels[c].rows; ++i)
        {
            for (int j = 0; j < channels[c].cols; ++j)
            {
                int newVal = floor(channels[c].at<uchar>(i, j) / thresholds[c]) * thresholds[c];
                channels[c].at<uchar>(i, j) = saturate_cast<uchar>(newVal);
            }
        }
    }

    // Juntar os canais para formar a imagem quantizada
    merge(channels, image);

    return image;
}

int main()
{
    // Caminho para a imagem de entrada
    string image_path = "C:/Projetos/ImageProcessing/ImageDetection/src/images/lenna.png";
    Mat image = imread(image_path);

    // Verifica se a imagem foi carregada corretamente
    if (image.empty())
    {
        cout << "Erro ao carregar a imagem colorida!" << endl;
        return -1;
    }

    // Define o número de bits para quantização
    int num_bits = 2;

    Mat IMG_QTZ_ADAP_RGB = QTZ_ADAP_RGB(image, num_bits);
    Mat IMG_QTZ_ADAP_GRAY = QTZ_ADAP_GRAY(image, num_bits);
    Mat IMG_QTZ_UNI_RGB = QTZ_UNI_RGB(image, num_bits);
    Mat IMG_QTZ_UNI_GRAY = QTZ_UNI_GRAY(image, num_bits);

    // Exibe as imagens originais e as imagens quantizadas
    imshow("Imagem Original - Colorida", image);
    imshow("Imagem Quantizada Uniforme - Colorida", IMG_QTZ_UNI_RGB);
    imshow("Imagem Quantizada Uniforme - Escala de Cinza", IMG_QTZ_UNI_GRAY);
    imshow("Imagem Quantizada Adaptativa - Colorida", IMG_QTZ_ADAP_RGB);
    imshow("Imagem Quantizada Adaptativa - Escala de Cinza", IMG_QTZ_ADAP_GRAY);
    waitKey(0);

    destroyAllWindows();

    return 0;
}

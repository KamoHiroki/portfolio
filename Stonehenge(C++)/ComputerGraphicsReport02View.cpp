
// ComputerGraphicsReport02View.cpp : CComputerGraphicsReport02View クラスの実装
//

#include "stdafx.h"
// SHARED_HANDLERS は、プレビュー、縮小版、および検索フィルター ハンドラーを実装している ATL プロジェクトで定義でき、
// そのプロジェクトとのドキュメント コードの共有を可能にします。
#ifndef SHARED_HANDLERS
#include "ComputerGraphicsReport02.h"
#endif

#include "ComputerGraphicsReport02Doc.h"
#include "ComputerGraphicsReport02View.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif



#define _USE_MATH_DEFINES
#include <math.h>

const double delta = 40;
const double aspectRatio = 1.5;
const int WIDTH = 512;

const double d = 1;
const double width = tan((M_PI / 180.0)*delta / 2);
const double height = width / aspectRatio;
const int HEIGHT = int(WIDTH / aspectRatio);

const double zfar = 100, znear = 0.5;

struct Point3 {
	double x, y, z;
};

struct LineElement {
	int start, end;
};

void drawLine(CDC* pDC, CPoint p, CPoint q)
{
	pDC->MoveTo(p);
	pDC->LineTo(q);
}

double proMat[4][4];

void setProjection()
{
	proMat[0][0] = d; proMat[0][1] = 0; proMat[0][2] = 0;  proMat[0][3] = 0;
	proMat[1][0] = 0; proMat[1][1] = d; proMat[1][2] = 0;  proMat[1][3] = 0;
	proMat[2][0] = 0; proMat[2][1] = 0; proMat[2][2] = zfar / (zfar - znear); proMat[2][3] = -zfar* znear / (zfar - znear);
	proMat[3][0] = 0; proMat[3][1] = 0; proMat[3][2] = 1; proMat[3][3] = 0;
}

double cMat[4][4];
void setCtrans()
{
	cMat[0][0] = WIDTH / width; cMat[0][1] = 0; cMat[0][2] = 0; cMat[0][3] = WIDTH + 0.5;
	cMat[1][0] = 0; cMat[1][1] = HEIGHT / height; cMat[1][2] = 0; cMat[1][3] = HEIGHT + 0.5;
	cMat[2][0] = 0; cMat[2][1] = 0; cMat[2][2] = 1; cMat[2][3] = 0;
	cMat[3][0] = 0; cMat[3][1] = 0; cMat[3][2] = 0; cMat[3][3] = 1;
}

double pMat[4][4];

void setPtrans(Point3 V)
{
	pMat[0][0] = 1; pMat[0][1] = 0; pMat[0][2] = 0; pMat[0][3] = -V.x;
	pMat[1][0] = 0; pMat[1][1] = 1; pMat[1][2] = 0; pMat[1][3] = -V.y;
	pMat[2][0] = 0; pMat[2][1] = 0; pMat[2][2] = 1; pMat[2][3] = -V.z;
	pMat[3][0] = 0; pMat[3][1] = 0; pMat[3][2] = 0; pMat[3][3] = 1;
}

#define _USE_MATH_DEFINES
#include <math.h>

Point3 outerproduct(Point3 p, Point3 q)
{
	Point3 r;
	r.x = p.y*q.z - p.z*q.y;
	r.y = p.z*q.x - p.x*q.z;
	r.z = p.x*q.y - p.y*q.x;
	return r;
}

double size(Point3 p)
{
	return sqrt(p.x*p.x + p.y*p.y + p.z*p.z);
}

double rMat[4][4];

void setRtrans(Point3 D, Point3 U)
{
	double nxx, nxy, nxz;
	double nyx, nyy, nyz;
	double nzx, nzy, nzz;

	double DSIZE = size(D);
	D.x /= DSIZE; D.y /= DSIZE; D.z /= DSIZE;
	nzx = D.x; nzy = D.y; nzz = D.z;

	Point3 UD = outerproduct(D, U);
	double UDSIZE = size(UD);
	UD.x /= UDSIZE; UD.y /= UDSIZE; UD.z /= UDSIZE;
	nxx = UD.x; nxy = UD.y; nxz = UD.z;

	Point3 NY = outerproduct(D, UD);
	nyx = NY.x; nyy = NY.y; nyz = NY.z;

	rMat[0][0] = nxx; rMat[0][1] = nxy; rMat[0][2] = nxz; rMat[0][3] = 0;
	rMat[1][0] = nyx; rMat[1][1] = nyy; rMat[1][2] = nyz; rMat[1][3] = 0;
	rMat[2][0] = nzx; rMat[2][1] = nzy; rMat[2][2] = nzz; rMat[2][3] = 0;
	rMat[3][0] = 0; rMat[3][1] = 0; rMat[3][2] = 0; rMat[3][3] = 1;
}

void matrixProduct(double p[4][4], double q[4][4], double r[4][4])
{
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
		{
			p[i][j] = 0.0;
			for (int k = 0; k < 4; k++) p[i][j] += q[i][k] * r[k][j];
		}
}

double tMat[4][4];
void setTrans(Point3 V, Point3 D, Point3 U)
{
	setPtrans(V);
	setRtrans(D, U);
	setProjection();
	setCtrans();

	double tmp[4][4], tmp2[4][4];
	matrixProduct(tmp, rMat, pMat);
	matrixProduct(tmp2, proMat, tmp);
	matrixProduct(tMat, cMat, tmp2);
}

Point3 trans(Point3 p)
{
	double x[4], y[4];
	x[0] = p.x; x[1] = p.y; x[2] = p.z; x[3] = 1;
	for (int i = 0; i < 4; i++)
	{
		y[i] = 0;
		for (int k = 0; k < 4; k++) y[i] += tMat[i][k] * x[k];
	}

	Point3 r;
	r.x = y[0] / y[3]; r.y = y[1] / y[3]; r.z = y[2] / y[3];
	return r;
}

struct Shape
{
	int NP3S, NLES;
	Point3		*p3s;
	LineElement	*les;
	CPoint		*cps;
	boolean		*flg;
};

Shape makePrism(int N)
{
	Shape c;
	c.NP3S = 2 * N;
	c.NLES = 3 * N;
	c.p3s = new Point3[c.NP3S];
	c.les = new LineElement[c.NLES];
	c.cps = new CPoint[c.NP3S];
	c.flg = new boolean[c.NP3S];

	for (int i = 0; i < N; i++)
	{
		c.p3s[i].x = c.p3s[i + N].x = cos((M_PI / 180) * (360.0 / N) * i);
		c.p3s[i].y = c.p3s[i + N].y = sin((M_PI / 180) * (360.0 / N) * i);
		c.p3s[i].z = 0; c.p3s[i + N].z = 1;
	}
	for (int i = 0; i < N; i++)
	{
		c.les[i].start = i; c.les[i].end = (i + 1) % N;
		c.les[i + N].start = i + N; c.les[i + N].end = (i + 1) % N + N;
		c.les[i + 2 * N].start = i; c.les[i + 2 * N].end = i + N;
	}
	return c;
}

void deleteShape(Shape* sp)
{
	delete[] sp->p3s;
	delete[] sp->les;
	delete[] sp->cps;
	delete[] sp->flg;
}

void drawShape(CDC* pDC, Shape* sp)
{
	for (int i = 0; i < sp->NP3S; i++)
	{
		Point3 cp = trans(sp->p3s[i]);
		if ((0 <= cp.z) && (cp.z <= 1))
		{
			sp->flg[i] = true;
			sp->cps[i].x = int(cp.x); sp->cps[i].y = int(cp.y);
		}
		else
		{
			sp->flg[i] = false;
		}
	}

	for (int i = 0; i < sp->NLES; i++)
	{
		if (sp->flg[sp->les[i].start] && sp->flg[sp->les[i].end])
		{
			drawLine(pDC, sp->cps[sp->les[i].start], sp->cps[sp->les[i].end]);
		}
	}
}

void stretch(Shape* sp, double rx, double ry, double rz)
{
	for (int i = 0; i < sp->NP3S; i++)
	{
		sp->p3s[i].x *= rx; sp->p3s[i].y *= ry; sp->p3s[i].z *= rz;
	}
}
void move(Shape* sp, double vx, double vy, double vz)
{
	for (int i = 0; i < sp->NP3S; i++)
	{
		sp->p3s[i].x += vx; sp->p3s[i].y += vy; sp->p3s[i].z += vz;
	}
}
void rotateX(Shape* sp, double a)
{
	double costh = cos((M_PI / 180)*a);
	double sinth = sin((M_PI / 180)*a);
	for (int i = 0; i < sp->NP3S; i++)
	{
		double y = costh*sp->p3s[i].y - sinth*sp->p3s[i].z;
		double z = sinth*sp->p3s[i].y + costh*sp->p3s[i].z;
		sp->p3s[i].y = y; sp->p3s[i].z = z;
	}
}
void rotateY(Shape* sp, double a)
{
	double costh = cos((M_PI / 180)*a);
	double sinth = sin((M_PI / 180)*a);
	for (int i = 0; i < sp->NP3S; i++)
	{
		double x = costh*sp->p3s[i].x - sinth*sp->p3s[i].z;
		double z = sinth*sp->p3s[i].x + costh*sp->p3s[i].z;
		sp->p3s[i].x = x; sp->p3s[i].z = z;
	}
}
void rotateZ(Shape* sp, double a)
{
	double costh = cos((M_PI / 180)*a);
	double sinth = sin((M_PI / 180)*a);
	for (int i = 0; i < sp->NP3S; i++)
	{
		double x = costh*sp->p3s[i].x - sinth*sp->p3s[i].y;
		double y = sinth*sp->p3s[i].x + costh*sp->p3s[i].y;
		sp->p3s[i].x = x; sp->p3s[i].y = y;
	}
}





// CComputerGraphicsReport02View

IMPLEMENT_DYNCREATE(CComputerGraphicsReport02View, CView)

BEGIN_MESSAGE_MAP(CComputerGraphicsReport02View, CView)
	// 標準印刷コマンド
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
END_MESSAGE_MAP()

// CComputerGraphicsReport02View コンストラクション/デストラクション

CComputerGraphicsReport02View::CComputerGraphicsReport02View()
{
	// TODO: 構築コードをここに追加します。

}

CComputerGraphicsReport02View::~CComputerGraphicsReport02View()
{
}

BOOL CComputerGraphicsReport02View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: この位置で CREATESTRUCT cs を修正して Window クラスまたはスタイルを
	//  修正してください。

	return CView::PreCreateWindow(cs);
}

// CComputerGraphicsReport02View 描画

void CComputerGraphicsReport02View::OnDraw(CDC* pDC)
{
	Point3 V, D, U;
	//V.x = 0;  V.y = 0; V.z = 30;						    // z軸から
	V.x = 15;  V.y = -7.5; V.z = 3.1;						// 立っている位置
	D.x = 0.0 - V.x; D.y = 0.0 - V.y; D.z = 0 - V.z;	// 見ている方角
	//U.x = 0; U.y = 1; U.z = 0;						    // z軸から
	U.x = 0; U.y = 0; U.z = 1;							// 上方向のベクトル

	setTrans(V, D, U);

	
	Shape shape1 = makePrism(4);
	stretch(&shape1, 0.2, 0.2, 1.0);
	rotateZ(&shape1, 45);

	Shape shape2 = makePrism(4); 
	stretch(&shape2, 0.2, 0.2, 0.9);
	rotateZ(&shape2, 45);
	rotateX(&shape2, -90);


	int k = 0; // shape1の角度のパラメータ
	int	l = 1; // shape2の角度のパラメータ
	for (double i = 0; i < 2* M_PI; i+= M_PI/10) {

		// shape1を描画
		rotateZ(&shape1, 18 * k);
		move(&shape1, 3*cos(i), 3*sin(i), 0);
		
		drawShape(pDC, &shape1);
		
		move(&shape1, -3 * cos(i), -3 * sin(i), 0);
		rotateZ(&shape1, -18 * k);
		k++;


		// shape2を描画
		rotateZ(&shape2, 9 * l);
		move(&shape2, 3 * cos(i), 3 * sin(i), 1.0 + sqrt(0.02));

		drawShape(pDC, &shape2);

		move(&shape2, -3 * cos(i), -3 * sin(i), -1.0 - sqrt(0.02));
		rotateZ(&shape2, -9 * l);
		l += 2;

	}

	Shape shape3 = makePrism(4);
	stretch(&shape3, 0.2, 0.2, 1.0);
	rotateZ(&shape3, 45);

	Shape shape4 = makePrism(4);
	stretch(&shape4, 0.2, 0.2, 0.5);
	rotateZ(&shape4, 45);
	rotateX(&shape4, -90);

	int m = 0;
	int n = 1;
	
	for (double i = 0; i <= (14.0/9.0) * M_PI; i += M_PI / 9) {

		if (m % 3 != 0) {

			// shape3を描画
			rotateZ(&shape3, 20 * m);
			move(&shape3, 1.5 * cos(i), 1.5 * sin(i), 0);

			drawShape(pDC, &shape3);

			move(&shape3, -1.5 * cos(i), -1.5 * sin(i), 0);
			rotateZ(&shape3, -20 * m);

			if (m % 3 == 1) {

				// shape4を描画
				rotateZ(&shape4,  10*n);
				move(&shape4, 1.5 * cos(i), 1.5 * sin(i), 1.0 + sqrt(0.02));

				drawShape(pDC, &shape4);

				move(&shape4, -1.5 * cos(i), -1.5 * sin(i), -1.0 - sqrt(0.02));
				rotateZ(&shape4,  -10*n);
			}
		}
		m++;
		n += 2;
		
	}

	Shape shape5 = makePrism(4);
	stretch(&shape5, 0.2, 0.2, 0.5);
	rotateZ(&shape5, 15);
	move(&shape5, -0.4, 0.3, 0);
	drawShape(pDC, &shape5);

	deleteShape(&shape1);
	deleteShape(&shape2);
	deleteShape(&shape3);
	deleteShape(&shape4);
	deleteShape(&shape5);
}


// CComputerGraphicsReport02View 印刷

BOOL CComputerGraphicsReport02View::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 既定の印刷準備
	return DoPreparePrinting(pInfo);
}

void CComputerGraphicsReport02View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 印刷前の特別な初期化処理を追加してください。
}

void CComputerGraphicsReport02View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 印刷後の後処理を追加してください。
}


// CComputerGraphicsReport02View 診断

#ifdef _DEBUG
void CComputerGraphicsReport02View::AssertValid() const
{
	CView::AssertValid();
}

void CComputerGraphicsReport02View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CComputerGraphicsReport02Doc* CComputerGraphicsReport02View::GetDocument() const // デバッグ以外のバージョンはインラインです。
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CComputerGraphicsReport02Doc)));
	return (CComputerGraphicsReport02Doc*)m_pDocument;
}
#endif //_DEBUG


// CComputerGraphicsReport02View メッセージ ハンドラー

#ifndef _ZHAOGANG_LIST_H
#define _ZHAOGANG_LIST_H

/* ������ڵ� */
typedef struct Sl_Node_s
{
    struct Sl_Node_s *pstNext;
}SL_NODE_S;

/* ������ͷ */
typedef struct sl_Head_s
{
    SL_NODE_S *pstFirst;
}SL_HEAD_S;

VOID LIST_SL_Init(SL_HEAD_S *pstHead);
VOID LIST_SL_AddHead(SL_HEAD_S *pstHead, SL_NODE_S *pstNode);
VOID LIST_SL_AddBefore(SL_NODE_S *pstCurNode, SL_NODE_S *pstNode);
VOID LIST_SL_FreeAll(SL_HEAD_S *pstHead, VOID (*pFunc)(VOID *pstNode));

/* ����ȫ��ѭ������������,��Ϊ��һ���ڵ㿿��һ�ڵ��ȡ */
#define LIST_SL_FOREACH(head, pos) \
    for ((pos) = (head)->pstFirst; NULL != (pos); (pos) = (pos)->pstNext)

/* ��ȫ��ѭ������������ */
#define LIST_SL_FOREACH_SAFE(head, curNode, tempNode) \
	for (curNode = (head)->pstFirst; \
        ((NULL) != (curNode)) && (((tempNode) = (curNode)->pstNext), BOOL_TRUE); \
         (curNode) = (tempNode))


/* ����β���еĽڵ� */
typedef struct Stq_Node_s
{
    struct Stq_Node_s *pstNext;
}STQ_NODE_S;

/* ����β���е�ͷ */
typedef struct Stq_Head_s
{
    STQ_NODE_S *pstFirst;
    STQ_NODE_S *pstLast;
}STQ_HEAD_S;

VOID LIST_STQ_Init(STQ_HEAD_S *pstHead);

STQ_NODE_S* LIST_STQ_DelHead(STQ_HEAD_S *pstHead);
VOID LIST_STQ_AddTail(STQ_HEAD_S *pstHead, STQ_NODE_S *pstNode);
VOID LIST_STQ_FreeAll(STQ_HEAD_S *pstHead, VOID (*pFunc)(VOID *pstNode));

/* ����ȫ��ѭ��������β����,��Ϊ��һ���ڵ㿿��һ�ڵ��ȡ */
#define LIST_STQ_FOREACH(head, pos) \
    for ((pos) = (head)->pstFirst; NULL != (pos); (pos) = (pos)->pstNext)

/* ��ȫ��ѭ��������β���� */
#define LIST_STQ_FOREACH_SAFE(head, curNode, tempNode) \
	for (curNode = (head)->pstFirst; \
        ((NULL) != (curNode)) && (((tempNode) = (curNode)->pstNext), BOOL_TRUE); \
         (curNode) = (tempNode))


/* ˫����Ľڵ� */
typedef struct Dl_Node_s
{
    struct Dl_Node_s *pstNext;   /* ��һ���ڵ� */
    struct Dl_Node_s **ppstPrev;  /* ǰһ���ڵ�ĵ�ַ */
}DL_NODE_S;
typedef struct Dl_Head_s
{
    struct Dl_Node_s *pstFirst;
}DL_HEAD_S;
VOID LIST_DL_Init(DL_HEAD_S *pstHead);
VOID LIST_DL_AddHead(DL_HEAD_S *pstHead, DL_NODE_S *pstNode);
VOID LIST_DL_DelNode(DL_NODE_S *pstNode);
VOID LIST_DL_FreeAll(DL_HEAD_S *pstHead, VOID (*pFunc)(VOID *pstNode));

#define LIST_DL_FOREACH_SAFE(head, curNode, tempNode) \
	for (curNode = (head)->pstFirst; \
        ((NULL) != (curNode)) && (((tempNode) = (curNode)->pstNext), BOOL_TRUE); \
         (curNode) = (tempNode))

/* ˫��β���еĽڵ� */
typedef struct Dtq_Node_s
{
    struct Dtq_Node_s *pstNext;
    struct Dtq_Node_s *pstPrev;
}DTQ_NODE_S;

typedef struct Dtq_Head_s
{
    DTQ_NODE_S stHead;
}DTQ_HEAD_S;


VOID LIST_DTQ_Init(DTQ_HEAD_S *pstHead);

#endif

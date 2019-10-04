# paging-management
模擬頁置換演算法，並依照不同的頁置換法輸出每一個時間片段頁的使用情況，並輸出page fault、page replaces、page frames。

	input:
	5
	70120304230321201701
	
	output:
	--------------FIFO-----------------------
	7	7	F
	0	07	F
	1	107	F
	2	2107	F
	0	2107	
	3	32107	F
	0	32107	
	4	43210	F
	2	43210	
	3	43210	
	0	43210	
	3	43210	
	2	43210	
	1	43210	
	2	43210	
	0	43210	
	1	43210	
	7	74321	F
	0	07432	F
	1	10743	F
	Page Fault = 9  Page Replaces = 4  Page Frames = 5

	--------------LRU-----------------------
	7	7	F
	0	07	F
	1	107	F
	2	2107	F
	0	0217	
	3	30217	F
	0	03217	
	4	40321	F
	2	24031	
	3	32401	
	0	03241	
	3	30241	
	2	23041	
	1	12304	
	2	21304	
	0	02134	
	1	10234	
	7	71023	F
	0	07123	
	1	10723	
	Page Fault = 7  Page Replaces = 2  Page Frames = 5

	--------------Additional Reference Bits-------------------
	7	7	F
	0	07	F
	1	107	F
	2	2107	F
	0	2107	
	3	32107	F
	0	32107	
	4	43210	F
	2	43210	
	3	43210	
	0	43210	
	3	43210	
	2	43210	
	1	43210	
	2	43210	
	0	43210	
	1	43210	
	7	73210	F
	0	73210	
	1	73210	
	Page Fault = 7  Page Replaces = 2  Page Frames = 5

	--------------Second chance Page-------------------------
	7	7	F
	0	07	F
	1	107	F
	2	2107	F
	0	2107	
	3	32107	F
	0	32107	
	4	43210	F
	2	43210	
	3	43210	
	0	43210	
	3	43210	
	2	43210	
	1	43210	
	2	43210	
	0	43210	
	1	43210	
	7	74321	F
	0	07432	F
	1	10743	F
	Page Fault = 9  Page Replaces = 4  Page Frames = 5

	--------------Least Frequently Used Page Replacement --------------------
	7	7	F
	0	07	F
	1	107	F
	2	2107	F
	0	2107	
	3	32107	F
	0	32107	
	4	43210	F
	2	43210	
	3	43210	
	0	43210	
	3	43210	
	2	43210	
	1	43210	
	2	43210	
	0	43210	
	1	43210	
	7	73210	F
	0	73210	
	1	73210	
	Page Fault = 7  Page Replaces = 2  Page Frames = 5

	--------------Most Frequently Used Page Replacement ------------------------
	7	7	F
	0	07	F
	1	107	F
	2	2107	F
	0	0217	
	3	3217	F
	0	03217	F
	4	40321	F
	2	24031	
	3	32401	
	0	03241	
	3	30241	
	2	23041	
	1	12304	
	2	21304	
	0	02134	
	1	10234	
	7	71034	F
	0	07134	
	1	10734	
	Page Fault = 8  Page Replaces = 3  Page Frames = 5
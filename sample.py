from collections import Counter
class Solution:
   
    #Function to sort the array according to frequency of elements.
    def sortByFreq(self,arr):
        arr.sort()
        for i in range(len(arr)):
            arr[i]=arr[i]
        d=Counter(arr)
        d=dict(sorted(d.items(),key=lambda x:x[1], reverse=True))
        ans=[]
        #print(d)
        for i,j in d.items():
            ans.extend([i]*(j))
        return ans
a=Solution()
b=a.sortByFreq([2,3,9, 9, 9,5])
print(b)

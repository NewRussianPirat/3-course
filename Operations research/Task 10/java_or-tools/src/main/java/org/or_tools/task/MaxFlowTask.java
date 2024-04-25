package org.or_tools.task;

import com.google.ortools.Loader;
import com.google.ortools.graph.MaxFlow;

import java.util.LinkedList;
import java.util.Queue;

public final class MaxFlowTask {
    public static void main(String[] args) throws Exception {
        Loader.loadNativeLibraries();

        MaxFlow maxFlow = new MaxFlow();

        int source = 0;
        int sink = 5;
        int[] startNodes = new int[]{0, 0, 1, 2, 2, 3, 3, 4};
        int[] endNodes = new int[]{1, 2, 4, 1, 3, 4, 5, 5};
        int[] capacities = new int[]{2, 2, 2, 5, 7, 2, 3, 1};

        for (int i = 0; i < startNodes.length; ++i) {
            int arc = maxFlow.addArcWithCapacity(startNodes[i], endNodes[i], capacities[i]);
            if (arc != i) {
                throw new Exception("Internal error");
            }
        }

        MaxFlow.Status status = maxFlow.solve(source, sink);

        if (status == MaxFlow.Status.OPTIMAL) {
            System.out.println("\nMaximum flow: " + maxFlow.getOptimalFlow());
            System.out.println();
            System.out.println(" Arc    FLow / Capacity");
            for (int i = 0; i < maxFlow.getNumArcs(); ++i) {
                System.out.println(maxFlow.getTail(i) + " -> " + maxFlow.getHead(i) + "    "
                        + maxFlow.getFlow(i) + "  /  " + maxFlow.getCapacity(i));
            }
        } else {
            System.out.println("Solving the max flow problem failed. Solver status: " + status);
        }

        //BFS
        Queue<Integer> queue = new LinkedList<>();
        boolean[] visited = new boolean[maxFlow.getNumNodes()];
        queue.add(source);
        visited[source] = true;
        while (!queue.isEmpty()) {
            int node = queue.poll();
            for (int arc = 0; arc < maxFlow.getNumArcs(); ++arc) {
                if (maxFlow.getTail(arc) == node && maxFlow.getCapacity(arc) > maxFlow.getFlow(arc)) {
                    int nextNode = maxFlow.getHead(arc);
                    if (!visited[nextNode]) {
                        visited[nextNode] = true;
                        queue.add(nextNode);
                    }
                }
            }
        }

        System.out.println("\nMinimum-cut arcs:");
        for (int arc = 0; arc < maxFlow.getNumArcs(); ++arc) {
            if (visited[maxFlow.getTail(arc)] && !visited[maxFlow.getHead(arc)]) {
                System.out.println(maxFlow.getTail(arc) + " -> " + maxFlow.getHead(arc));
            }
        }
    }

    private MaxFlowTask() {
    }
}
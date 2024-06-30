<template>
  <div class="container">
    <h3 class="text-2xl">{{formattedTime.time}}</h3>
    <h2 class="text-lg">{{formattedTime.date}}</h2>
  </div>
</template>
<script setup lang="ts">
import useTime from '@/composables/useTime'

const { formattedTime } = useTime()

</script>
<style lang="scss" scoped>
.container {
  display: flex;
  align-items: center;
  flex-direction: column;
  gap: 8px;
}
</style>